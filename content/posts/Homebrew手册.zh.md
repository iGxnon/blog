---
title: Homebrew手册
date: 2023-02-08
extra:
  add_toc: true
taxonomies:
  categories:
    - 笔记
  tags:
    - brew
---

## Homebrew手册

经常会使用到 `homebrew` 去安装一些软件，每次都迷迷糊糊的敲几个命令，出现问题了就得 google 找原因，想到这于是写一个手册方便查找

> 其实 `man brew` 就可以快速了解了

## 术语

`homebrew` 有一些奇奇怪怪的术语：（可以看出作者非常喜欢<span class="emoji">🍺</span>了

| 术语     | 释义            | 含义                              |
| -------- | --------------- | --------------------------------- |
| homebrew | 自酿啤酒        | 软件主体                          |
| formula  | 配方            | 包的描述文件(Ruby编写)            |
| cellar   | 酒窖            | 安装好后所在的目录                |
| cask     | 酒桶(木质)      | 图形化软件包(macOS native 的软件) |
| tap      | (动作)取酒      | 下载源                            |
| keg      | 酒桶(不锈钢质?) | 某个包所在的具体目录              |
| bottle   | 酒瓶            | 预先编译好的包                    |
| rack     | 架子(放keg的)   | 放置某个包(keg)所有版本的目录     |
| caskroom | 酒房(放cask的)  | 放置 cask 的目录                  |

可以看出 `homebrew` 由以下层级关系组成：

- `cellar`
  - `rack`
    - `keg` <- (`bottle` or build from source by refering `formula`)
- `caskroom`
  - `cask` <- (by refering `formula`)

keg-only

- 某些不会链接到特定位置的包

## 常用命令

### `brew install/uninstall`

- 使用方式：`brew install sl` 即可安装 `sl` 小火车了，同理 `uninstall` 删除

- 它干了什么？

  上面的例子里，它会查找记录了 `sl` 软件的 `formula`，根据描述文件下载对应的软件，并链接到合适位置上

- 使用 `--cask` 即可安装 cask

### `brew search`

- 查找一个包

### `brew doctor`

- 检查 `homebrew` 安装情况

### `brew cleanup`

- 清理所有软件的历史版本
- `brew cleanup <软件>` 清理特定软件的历史版本

### `brew autoremove`

- 删掉一些不再使用的依赖包

### `brew update`

- 从 git 仓库内拉取最新的 `formulae`

### `brew upgrade`

- 更新某个包

### `brew info`

- 查看某个包的具体信息

### `brew tap`

- 添加一个下载源

## 换源

>  `homebrew` 源经常更新，换第三方源可能版本跟不上，如果是实在访问 github 太慢再考虑换源

cd 到 `brew --repo` 下更改所有 `.git` 下的 git remote 即可

## 注意

`homebrew` 的 bin 目录应该要出现在 `/usr/bin` 等系统 bin 前面，这可以让 `homebrew` 将系统的包同时管理