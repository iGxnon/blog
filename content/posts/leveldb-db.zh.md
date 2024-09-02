---
title: 《源码解读系列》LevelDB db
date: 2024-08-25
description: 工程上的权衡
extra:
  add_toc: true
taxonomies:
  categories: ["编程"]
  tags: ["LevelDB", "源码解读系列"]
---

## DB format

- 构成 DB 的组件

> 一些组件比较简单，具体实现就跳过了

### Config

- 编译期配置

```c++
namespace config {
// 最多 7 层，每层 10^L MB，总共 ~10TB
static const int kNumLevels = 7;

// 下面是 Compaction 相关的配置

// Level-0 compaction is started when we hit this many files.
static const int kL0_CompactionTrigger = 4;

// Soft limit on number of level-0 files.  We slow down writes at this point.
static const int kL0_SlowdownWritesTrigger = 8;

// Maximum number of level-0 files.  We stop writes at this point.
static const int kL0_StopWritesTrigger = 12;

// Maximum level to which a new compacted memtable is pushed if it
// does not create overlap.  We try to push to level 2 to avoid the
// relatively expensive level 0=>1 compactions and to avoid some
// expensive manifest file operations.  We do not push all the way to
// the largest level since that can generate a lot of wasted disk
// space if the same key space is being repeatedly overwritten.
static const int kMaxMemCompactLevel = 2;

// Approximate gap in bytes between samples of data read during iteration.
static const int kReadBytesPeriod = 1048576;
}
```

### InternalKey

- 格式 `[user_key]-[seq_num + type]`，其中 `[seq_num + type]` 占 8B，`seq_num` 最多有 7B 表示，`type` 最多有 1B 表示

### InternalKeyComparator

- 包裹了一层用户提供的 Comparator，能在 user key 的基础上继续比较 internal key 多出的部分
- 比较顺序是：user comparator -> seq_num(降序) -> type(降序)

### Type

- `kTypeDeletion = 0x0, kTypeValue = 0x1`
- 还有一个常数 `kValueTypeForSeek = kTypeValue`，用于对 user key seek 时构建 internal key
  - 因为 comparator 对 type 是降序的，所以 value 会排在 deletion 前面，seek 到 value 那

### InternalFilterPolicy

- 用于把 internal key 的尾巴去掉再构建 FilterPolicy

实现

```c++
void InternalFilterPolicy::CreateFilter(const Slice* keys, int n,
                                        std::string* dst) const {
  // We rely on the fact that the code in table.cc does not mind us
  // adjusting keys[].
  // 额...，行吧，希望有人不会踩这个坑。
  Slice* mkey = const_cast<Slice*>(keys);
  for (int i = 0; i < n; i++) {
    mkey[i] = ExtractUserKey(keys[i]);
    // TODO(sanjay): Suppress dups?
  }
  user_policy_->CreateFilter(keys, n, dst);
}
```

### LookupKey

- 各种 key 的大杂烩
  - memtable_key
  - internal_key
  - user_key
- 小 key 会分配到栈上

定义

```c++
// A helper class useful for DBImpl::Get()
class LookupKey {
 public:
  // Initialize *this for looking up user_key at a snapshot with
  // the specified sequence number.
  LookupKey(const Slice& user_key, SequenceNumber sequence);

  LookupKey(const LookupKey&) = delete;
  LookupKey& operator=(const LookupKey&) = delete;

  ~LookupKey();

  // Return a key suitable for lookup in a MemTable.
  Slice memtable_key() const { return Slice(start_, end_ - start_); }

  // Return an internal key (suitable for passing to an internal iterator)
  Slice internal_key() const { return Slice(kstart_, end_ - kstart_); }

  // Return the user key
  Slice user_key() const { return Slice(kstart_, end_ - kstart_ - 8); }

 private:
  // We construct a char array of the form:
  //    klength  varint32               <-- start_
  //    userkey  char[klength]          <-- kstart_
  //    tag      uint64
  //                                    <-- end_
  // The array is a suitable MemTable key.
  // The suffix starting with "userkey" can be used as an InternalKey.
  const char* start_;
  const char* kstart_;
  const char* end_;
  char space_[200];  // Avoid allocation for short keys，用于小 key 的分配
};

inline LookupKey::~LookupKey() {
  if (start_ != space_) delete[] start_; // 如果分配在堆上的话就手动清理
}
```

实现

```c++
LookupKey::LookupKey(const Slice& user_key, SequenceNumber s) {
  size_t usize = user_key.size();
  size_t needed = usize + 13;  // A conservative estimate
  char* dst;
  if (needed <= sizeof(space_)) {
    dst = space_; // 小 key 就在栈上分配
  } else {
    dst = new char[needed];
  }
  start_ = dst;
  dst = EncodeVarint32(dst, usize + 8);
  kstart_ = dst;
  std::memcpy(dst, user_key.data(), usize);
  dst += usize;
  EncodeFixed64(dst, PackSequenceAndType(s, kValueTypeForSeek));
  dst += 8;
  end_ = dst;
}
```

## Memtable

- skip list 的一层包装
- 维护了一个 arena，作为 skip list 的存储
- 引用计数内存管理
- 删除是 Add 一个 DeletionType 的 key

定义

```c++
class InternalKeyComparator;
class MemTableIterator;

class MemTable {
 public:
  // MemTables are reference counted.  The initial reference count
  // is zero and the caller must call Ref() at least once.
  explicit MemTable(const InternalKeyComparator& comparator);

  // 只能引用传递
  MemTable(const MemTable&) = delete;
  MemTable& operator=(const MemTable&) = delete;

  // Increase reference count.
  void Ref() { ++refs_; }

  // Drop reference count.  Delete if no more references exist.
  void Unref() {
    --refs_;
    assert(refs_ >= 0);
    if (refs_ <= 0) {
      delete this;
    }
  }

  // Returns an estimate of the number of bytes of data in use by this
  // data structure. It is safe to call when MemTable is being modified.
  // 来自 arena 的内存占用估计
  size_t ApproximateMemoryUsage();

  // Return an iterator that yields the contents of the memtable.
  //
  // The caller must ensure that the underlying MemTable remains live
  // while the returned iterator is live.  The keys returned by this
  // iterator are internal keys encoded by AppendInternalKey in the
  // db/format.{h,cc} module.
  Iterator* NewIterator();

  // Add an entry into memtable that maps key to value at the
  // specified sequence number and with the specified type.
  // Typically value will be empty if type==kTypeDeletion.
  void Add(SequenceNumber seq, ValueType type, const Slice& key,
           const Slice& value);

  // If memtable contains a value for key, store it in *value and return true.
  // If memtable contains a deletion for key, store a NotFound() error
  // in *status and return true.
  // Else, return false.
  bool Get(const LookupKey& key, std::string* value, Status* s);

 private:
  friend class MemTableIterator;
  friend class MemTableBackwardIterator;

  struct KeyComparator { // 给 SkipList 用来比较的
    const InternalKeyComparator comparator;
    explicit KeyComparator(const InternalKeyComparator& c) : comparator(c) {}
    int operator()(const char* a, const char* b) const; // 重载这个运算符有类似 lambda 表达式的效果
  };
  
  // Memtable 的真身
  typedef SkipList<const char*, KeyComparator> Table;

  // 引用计数
  ~MemTable();  // Private since only Unref() should be used to delete it

  KeyComparator comparator_;
  int refs_;
  Arena arena_;
  Table table_;
};
```

实现

> 部分实现只是转发到 skip list 那，就略过了

```c++
// key 和 value 拼在一起放进 skip list 里
void MemTable::Add(SequenceNumber s, ValueType type, const Slice& key,
                   const Slice& value) {
  // Format of an entry is concatenation of:
  //  key_size     : varint32 of internal_key.size()
  //  key bytes    : char[user_key.size()]
  //  tag          : uint64((sequence << 8) | type)
  //  value_size   : varint32 of value.size()
  //  value bytes  : char[value.size()]
  size_t key_size = key.size();
  size_t val_size = value.size();
  size_t internal_key_size = key_size + 8; // 算上小尾巴
  const size_t encoded_len = VarintLength(internal_key_size) +
                             internal_key_size + VarintLength(val_size) +
                             val_size;
  char* buf = arena_.Allocate(encoded_len);
  char* p = EncodeVarint32(buf, internal_key_size);
  std::memcpy(p, key.data(), key_size);
  p += key_size;
  EncodeFixed64(p, (s << 8) | type);
  p += 8;
  p = EncodeVarint32(p, val_size);
  std::memcpy(p, value.data(), val_size);
  assert(p + val_size == buf + encoded_len);
  table_.Insert(buf);
}

bool MemTable::Get(const LookupKey& key, std::string* value, Status* s) {
  Slice memkey = key.memtable_key();
  Table::Iterator iter(&table_);
  iter.Seek(memkey.data());
  if (iter.Valid()) {
    // entry format is:
    //    klength  varint32
    //    userkey  char[klength]
    //    tag      uint64
    //    vlength  varint32
    //    value    char[vlength]
    // Check that it belongs to same user key.  We do not check the
    // sequence number since the Seek() call above should have skipped
    // all entries with overly large sequence numbers.
    const char* entry = iter.key();
    uint32_t key_length;
    const char* key_ptr = GetVarint32Ptr(entry, entry + 5, &key_length);
    // 只比 user key，这里可以拿到关于这个 user key 的最大的 seq_num 的 value
    if (comparator_.comparator.user_comparator()->Compare(
            Slice(key_ptr, key_length - 8), key.user_key()) == 0) {
      // Correct user key
      const uint64_t tag = DecodeFixed64(key_ptr + key_length - 8);
      switch (static_cast<ValueType>(tag & 0xff)) {
        case kTypeValue: {
          Slice v = GetLengthPrefixedSlice(key_ptr + key_length);
          value->assign(v.data(), v.size());
          return true;
        }
        case kTypeDeletion:
          *s = Status::NotFound(Slice());
          return true;
      }
    }
  }
  return false;
}
```

## DB

- DB 的实现有一把大锁保护着 memtable 等数据结构，不过锁定的范围不宽，很多耗时操作可以并发的。

### TableCache

- Table 级别的 cache（区别于 table 内 block 级别的 cache）

定义

```c++
class TableCache {
 public:
  TableCache(const std::string& dbname, const Options& options, int entries);

  TableCache(const TableCache&) = delete;
  TableCache& operator=(const TableCache&) = delete;

  ~TableCache();

  // Return an iterator for the specified file number (the corresponding
  // file length must be exactly "file_size" bytes).  If "tableptr" is
  // non-null, also sets "*tableptr" to point to the Table object
  // underlying the returned iterator, or to nullptr if no Table object
  // underlies the returned iterator.  The returned "*tableptr" object is owned
  // by the cache and should not be deleted, and is valid for as long as the
  // returned iterator is live.
  Iterator* NewIterator(const ReadOptions& options, uint64_t file_number,
                        uint64_t file_size, Table** tableptr = nullptr);

  // If a seek to internal key "k" in specified file finds an entry,
  // call (*handle_result)(arg, found_key, found_value).
  Status Get(const ReadOptions& options, uint64_t file_number,
             uint64_t file_size, const Slice& k, void* arg,
             void (*handle_result)(void*, const Slice&, const Slice&));

  // Evict any entry for the specified file number
  void Evict(uint64_t file_number);

 private:
  Status FindTable(uint64_t file_number, uint64_t file_size, Cache::Handle**);

  Env* const env_;
  const std::string dbname_;
  const Options& options_;
  Cache* cache_;
};
```

实现

```c++
// cache entry
struct TableAndFile {
  RandomAccessFile* file;
  Table* table;
};

// 释放这个 table 的内存，用于给 cache 淘汰某个 table 使用
static void DeleteEntry(const Slice& key, void* value) {
  TableAndFile* tf = reinterpret_cast<TableAndFile*>(value);
  delete tf->table;
  delete tf->file;
  delete tf;
}

// 把 table 放回缓存管理，有 cache 时的释放过程
static void UnrefEntry(void* arg1, void* arg2) {
  Cache* cache = reinterpret_cast<Cache*>(arg1);
  Cache::Handle* h = reinterpret_cast<Cache::Handle*>(arg2);
  cache->Release(h);
}

TableCache::TableCache(const std::string& dbname, const Options& options,
                       int entries)
    : env_(options.env),
      dbname_(dbname),
      options_(options),
      cache_(NewLRUCache(entries)) {}

TableCache::~TableCache() { delete cache_; }

Status TableCache::FindTable(uint64_t file_number, uint64_t file_size,
                             Cache::Handle** handle) {
  Status s;
  char buf[sizeof(file_number)];
  EncodeFixed64(buf, file_number);
  Slice key(buf, sizeof(buf)); // cache key 是 file number
  *handle = cache_->Lookup(key);

  if (*handle == nullptr) { // cache miss 了，读取 table
    std::string fname = TableFileName(dbname_, file_number); // <db_name>/<file_number>.ldb
    RandomAccessFile* file = nullptr;
    Table* table = nullptr;
    s = env_->NewRandomAccessFile(fname, &file);
    if (!s.ok()) {
      std::string old_fname = SSTTableFileName(dbname_, file_number);
      if (env_->NewRandomAccessFile(old_fname, &file).ok()) { // 兼容老文件名称
        s = Status::OK();
      }
    }
    if (s.ok()) {
      s = Table::Open(options_, file, file_size, &table);
    }

    if (!s.ok()) {
      assert(table == nullptr);
      delete file;
      // We do not cache error results so that if the error is transient,
      // or somebody repairs the file, we recover automatically.
    } else {
      TableAndFile* tf = new TableAndFile;
      tf->file = file;
      tf->table = table;
      *handle = cache_->Insert(key, tf, 1, &DeleteEntry);
    }
  }
  return s;
}

// 获得一个 Table 的 iterator
Iterator* TableCache::NewIterator(const ReadOptions& options,
                                  uint64_t file_number, uint64_t file_size,
                                  Table** tableptr) {
  if (tableptr != nullptr) {
    *tableptr = nullptr;
  }

  Cache::Handle* handle = nullptr;
  Status s = FindTable(file_number, file_size, &handle);
  if (!s.ok()) {
    return NewErrorIterator(s);
  }

  Table* table = reinterpret_cast<TableAndFile*>(cache_->Value(handle))->table;
  // 跳到 table 的 NewIterator
  Iterator* result = table->NewIterator(options);
  // iterator 被析构时归还给 cache，不同于某个 table 的 block cache 是可以没有的，table cache 总是有的
  result->RegisterCleanup(&UnrefEntry, cache_, handle);
  if (tableptr != nullptr) {
    *tableptr = table;
  }
  return result;
}

// 丑陋的回调
Status TableCache::Get(const ReadOptions& options, uint64_t file_number,
                       uint64_t file_size, const Slice& k, void* arg,
                       void (*handle_result)(void*, const Slice&,
                                             const Slice&)) {
  Cache::Handle* handle = nullptr;
  Status s = FindTable(file_number, file_size, &handle);
  if (s.ok()) {
    Table* t = reinterpret_cast<TableAndFile*>(cache_->Value(handle))->table;
    // 跳到 Table::InternalGet
    s = t->InternalGet(options, k, arg, handle_result);
    cache_->Release(handle);
  }
  return s;
}

// 可以手动驱逐 cache
void TableCache::Evict(uint64_t file_number) {
  char buf[sizeof(file_number)];
  EncodeFixed64(buf, file_number);
  cache_->Erase(Slice(buf, sizeof(buf)));
}
```

### Reader

#### Get

- 查询过程：`memtable -> immutable memtable -> sstable`
- 查询时可能会触发 compaction

```c++
Status DBImpl::Get(const ReadOptions& options, const Slice& key,
                   std::string* value) {
  Status s;
  MutexLock l(&mutex_);
  SequenceNumber snapshot;
  if (options.snapshot != nullptr) {
    // 如果当前是快照读的话，按用户要的 seq_num 去读
    snapshot =
        static_cast<const SnapshotImpl*>(options.snapshot)->sequence_number();
  } else {
    // 不然按最新的读
    snapshot = versions_->LastSequence();
  }

  MemTable* mem = mem_;
  MemTable* imm = imm_; // 也会查正在 compaction 的 memtable
  Version* current = versions_->current(); // 最后从 sstable 里查
  mem->Ref();
  if (imm != nullptr) imm->Ref();
  current->Ref();

  bool have_stat_update = false;
  Version::GetStats stats;

  // Unlock while reading from files and memtables
  // 读 memtable 时解锁，leveldb skiplist 可以支持多个写入和一个读取并发
  {
    mutex_.Unlock();
    // First look in the memtable, then in the immutable memtable (if any).
    LookupKey lkey(key, snapshot);
    if (mem->Get(lkey, value, &s)) {
      // Done
    } else if (imm != nullptr && imm->Get(lkey, value, &s)) {
      // Done
    } else {
      s = current->Get(options, lkey, value, &stats);
      have_stat_update = true; // 标记一下，说明我们可能需要进行一次压缩了（memtable没有命中，太大了）
    }
    mutex_.Lock();
  }

  if (have_stat_update && current->UpdateStats(stats)) {
    // 可能得进行一次压缩
    MaybeScheduleCompaction();
  }
  mem->Unref();
  if (imm != nullptr) imm->Unref();
  current->Unref();
  return s;
}
```

### Writer

#### WriteBatch

- 内存中的和落盘时的数据完全一样
- 结构：`<seq_num><count>(<type><key>[<value>])*`
  - 前 8B `seq_num`，4B `count`，后面是 kv 记录或删除记录

实现就不看了，知道结构就简单了

#### Write

- 排队逐个批量写入
- 可能会把多个 batch 合并起来一起写

```c++
Status DBImpl::Write(const WriteOptions& options, WriteBatch* updates) {
  Writer w(&mutex_);
  w.batch = updates;
  w.sync = options.sync;
  w.done = false;

  MutexLock l(&mutex_);
  writers_.push_back(&w); // 排队 writer
  while (!w.done && &w != writers_.front()) {
    w.cv.Wait(); // 等前面的 writer 执行完
  }
  if (w.done) {
    return w.status;
  }

  // May temporarily unlock and wait.
  // 这里可能会等一会，如果 level0 的数据太多了的话
  Status status = MakeRoomForWrite(updates == nullptr);
  uint64_t last_sequence = versions_->LastSequence();
  Writer* last_writer = &w;
  if (status.ok() && updates != nullptr) {  // nullptr batch is for compactions
    WriteBatch* write_batch = BuildBatchGroup(&last_writer); // 构建一个 WriteBatch 开始写入了，可能会合并多个 batch
    WriteBatchInternal::SetSequence(write_batch, last_sequence + 1); // 递增的 sequence
    last_sequence += WriteBatchInternal::Count(write_batch);

    // Add to log and apply to memtable.  We can release the lock
    // during this phase since &w is currently responsible for logging
    // and protects against concurrent loggers and concurrent writes
    // into mem_.
    {
      mutex_.Unlock(); // 暂时把锁放掉，开始做文件IO了，但这里其实还是只有一个 writer 会进来，前面对所有的 writer 排队了
      status = log_->AddRecord(WriteBatchInternal::Contents(write_batch)); // WAL
      bool sync_error = false;
      if (status.ok() && options.sync) { // 如果开了 sync 的话
        status = logfile_->Sync(); // fsync 确保刷盘
        if (!status.ok()) {
          sync_error = true;
        }
      }
      if (status.ok()) {
        // 插入到 memtable
        status = WriteBatchInternal::InsertInto(write_batch, mem_);
      }
      mutex_.Lock();
      if (sync_error) { // fsync 写坏了，把 DB 标记成坏掉的状态
        // The state of the log file is indeterminate: the log record we
        // just added may or may not show up when the DB is re-opened.
        // So we force the DB into a mode where all future writes fail.
        RecordBackgroundError(status);
      }
    }
    if (write_batch == tmp_batch_) tmp_batch_->Clear(); // 如果攒了 batch 的话把攒的 batch 清理掉
	
    // 更新 last_seq
    versions_->SetLastSequence(last_sequence);
  }
  
  // 从writer列表里移除自己
  while (true) {
    Writer* ready = writers_.front();
    writers_.pop_front();
    if (ready != &w) {
      ready->status = status;
      ready->done = true;
      ready->cv.Signal();
    }
    if (ready == last_writer) break;
  }

  // Notify new head of write queue
  // 唤醒一个新的 writer
  if (!writers_.empty()) {
    writers_.front()->cv.Signal();
  }

  return status;
}
```

#### BuildBatchGroup

- 攒一攒 batch
  - `non-sync` 的 batch 不会和 `sync` 的合并，`non-sync` 的跑得快

实现

```c++

// REQUIRES: Writer list must be non-empty
// REQUIRES: First writer must have a non-null batch
WriteBatch* DBImpl::BuildBatchGroup(Writer** last_writer) {
  mutex_.AssertHeld();
  assert(!writers_.empty());
  Writer* first = writers_.front();
  WriteBatch* result = first->batch;
  assert(result != nullptr);

  size_t size = WriteBatchInternal::ByteSize(first->batch);

  // Allow the group to grow up to a maximum size, but if the
  // original write is small, limit the growth so we do not slow
  // down the small write too much.
  // 最大一次攒 1MB
  size_t max_size = 1 << 20;
  // 如果太小了就稍微少点，不让小批量的会被延迟
  if (size <= (128 << 10)) {
    max_size = size + (128 << 10);
  }

  *last_writer = first;
  std::deque<Writer*>::iterator iter = writers_.begin();
  ++iter;  // Advance past "first"
  for (; iter != writers_.end(); ++iter) {
    Writer* w = *iter;
    if (w->sync && !first->sync) { // non-sync + sync 是不允许的，先把 non-sync 跑得快的弄完，再写 sync 的
      // Do not include a sync write into a batch handled by a non-sync write.
      break;
    }

    if (w->batch != nullptr) {
      size += WriteBatchInternal::ByteSize(w->batch);
      if (size > max_size) {
        // Do not make batch too big
        // 不能再贪了
        break;
      }

      // Append to *result
      if (result == first->batch) {
        // Switch to temporary batch instead of disturbing caller's batch
        // 能不能不要这样写代码了！全局变量想开就开是吧
        result = tmp_batch_;
        assert(WriteBatchInternal::Count(result) == 0);
        WriteBatchInternal::Append(result, first->batch);
      }
      // 贴到 tmp_batch_ 后面去
      WriteBatchInternal::Append(result, w->batch);
    }
    *last_writer = w;
  }
  return result;
}
```

### Compaction

- LSM Tree 的精髓
- 去除掉冗余的记录和删除记录，保留一个 key 的最新 value

- 每个文件的大小限制 2MB，每一层文件总大小限制为 10^L MB
- 有个后台线程在做 Compaction，同时也可以手动 Compaction
  - 区别在于手动的是从传入的 key_range 中选取需要参与压缩的 table，而自动压缩是根据当前是否有超过限制的层级来确定参与压缩的 table

> When the size of level L exceeds its limit, we compact it in a background
>
> thread. The compaction picks a file from level L and all overlapping files from
>
> the next level L+1. Note that if a level-L file overlaps only part of a
>
> level-(L+1) file, the entire file at level-(L+1) is used as an input to the
>
> compaction and will be discarded after the compaction.  Aside: because level-0
>
> is special (files in it may overlap each other), we treat compactions from
>
> level-0 to level-1 specially: a level-0 compaction may pick more than one
>
> level-0 file in case some of these files overlap each other.

- 当每层达到对应的大小限制时会开始 Compaction
  - 每次选取一个文件和下一层所有重叠 key 的文件，把这个文件压缩到它下一层
  - Level 0 到 Level 1 的 Compaction 是特殊处理的（因为 Level 0 文件里的 key 可能会互相重叠）

> A compaction merges the contents of the picked files to produce a sequence of
>
> level-(L+1) files. We switch to producing a new level-(L+1) file after the
>
> current output file has reached the target file size (2MB). We also switch to a
>
> new output file when the key range of the current output file has grown enough
>
> to overlap more than ten level-(L+2) files.  This last rule ensures that a later
>
> compaction of a level-(L+1) file will not pick up too much data from
>
> level-(L+2).

- 构建压缩好的层时，如果每个文件超过了限制，则会另起一个文件

> Compactions for a particular level rotate through the key space. In more detail,
>
> for each level L, we remember the ending key of the last compaction at level L.
>
> The next compaction for level L will pick the first file that starts after this
>
> key (wrapping around to the beginning of the key space if there is no such
>
> file).

- 压缩过程会记录每一层压缩到的地方，下一次从这个地方后面开始压缩
- 如果到末尾了则会返回到开头继续

注：Compaction 的实现比较复杂，限于篇幅会放到后续文章中分析

### Snapshot

- 记录一个 sequence number，确定一个快照的时间点

定义：

```c++
// Abstract handle to particular state of a DB.
// A Snapshot is an immutable object and can therefore be safely
// accessed from multiple threads without any external synchronization.
// 一片空白的定义，只是拿来当一个 handle 使用
class LEVELDB_EXPORT Snapshot {
 protected:
  virtual ~Snapshot();
};
```

实现是一个按 sequence number 排序的有序链表，也就是说 sequence number 会作为 snapshot 的 handle

## Conclusion

DB 的主体实现大多是一些组合和并发控制，也是最多最复杂的部分，这里仅列举了构成 DB 的关键组件与操作。
