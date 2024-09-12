---
title: 抄来的ORM(
description: 读书人的事，能叫偷么？
date: 2022-01-29
taxonomies:
  categories:
    - 编程
  tags:
    - orm
    - 闲聊
---

# 源起

> 泡泡："温两碗酒，要一碟茴香豆"
> 
> 寒雨："你一定又偷人家东西了！"
> 
> 泡泡："你怎么这样凭空污人清白……"
> 
> 泡泡："窃 Code，不能叫偷，读书人的事，能叫偷么？"

(逃

由于接触数据库并不多，所以我操作数据库时大多数还是写个函数用SQL去操作，对 orm 的理解只处于"这是复杂的项目才会用到的技术"

不过当我看到这样的设计的时候

```kotlin
// 偷自 ColdRain
data class ExampleEntity(
    // PrimaryKey 不用写option
    @PrimaryKey(autoGenerate = true)
    @Column(name = "id", type = ColumnTypeSQL.INT)
    val id: Int? = null,
    @Column(name = "type", type = ColumnTypeSQL.TEXT, options = [ColumnOptionSQL.NOTNULL])
    val type: String,
    @Column(name = "user", type = ColumnTypeSQL.TEXT, options = [ColumnOptionSQL.NOTNULL])
    val user: String,
    @Column(name = "user", type = ColumnTypeSQL.TEXT, def = "null")
    val data: String?
)

interface ExampleDAO : IDao<ExampleEntity> {
    @Query("SELECT * FROM {table}")
    fun queryAll(): List<ExampleEntity>

    @Insert
    fun insert(entity: ExampleEntity)

    @Query("DELETE FROM {table}")
    fun delete()

    @Query("SELECT WHERE id > {id} FROM {table}")
    fun querySome(id: Int): List<ExampleEntity>

    // 使用TabooLib DSL语句控制
    @DSL
    fun selectUser(name: String): ExampleEntity? {
        return table.workspace(datasource) {
            select { where { "user" eq name } }
        }.firstOrNull {
            adaptResultSet()
        }
    }
}

object AppDatabase {
    /**
     * 也可通过ORMBuilder#buildFromConf(ConfigurationSection, String)直接构建
     */
    private val builder by lazy {
        ORMBuilder.newBuilder()
            .host("localhost")
            .port(3306)
            .user("root")
            .password("root")
            .database("database")
            .buildHost()
    }


    val exampleTableDao by lazy {
        // 表名 DAO类
        builder.build("exampleTable" ,ExampleDAO::class.java)
    }
}
```

？这好啊，DAO 都对象化了
不行，也整一个

# 过程

起初想用 golang 去实现一个类似这样的，毕竟 gorm 操作数据对象时还是函数式

但是发现

Go 没有注解！或者说，Go 函数没有注解，只有结构体那的 tag 可以反射拿数据

没有注解？反射 DAO 拿个啥？

解决思路

- 参考 [https://github.com/MarcGrol/golangAnnotations/wiki](https://github.com/MarcGrol/golangAnnotations/wiki)

很显然比较困难，以后再来

然后就写了一个 Java 版本，接入了 nk 这边挺多人用的 EasyMySQL

地址: [https://github.com/SmallasWater/EasyMySQL#orm-部分](https://github.com/SmallasWater/EasyMySQL#orm-部分)

## 遇到的一些坑

DAO 接口被代理后生成的一个对象，其中的 DAO 定义的 default 方法会被重写调用 invoke，然鹅 DAO 是一个接口没办法实例化，default 又被覆盖了，导致找不到对象执行 default，最终用动态字节码解决了（能用就行，谁在乎性能，逃

javassist 自带的 toClass 方法使用的是线程上下文的类加载器，这会导致好不容易构建好的字节码继承了一个"其他类型的 DAO"(jvm 判断类的类型取决于包名，类名和类加载器，前面两个都一样，但加载它们的类加载器不一样也会导致这两个类不是同一个类型)

javassist 提供的一个类加载器 Loader 使用的是自己的 classPool，里面找不到就抛异常，这打破了父类委托机制，虽然使用这个 Loader 可以解决 DAO 不一致问题，但是 DAO 里面要是引用了本项目其他类型的对象的话，会造成外面 new 的对象写不进动态字节码生成的类里，外面的类可以理解成 Loader 的父加载器加载的...而 Loader ban 掉了父类委托，一开始的方案是自己写个 ClassLoader 为 Loader "重建父类委托"，写了老长一串，后来发现 toClass 里面可以传入一个 ClassLoader...（Loader 爬
