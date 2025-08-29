****
# libcc

![platform](https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20macos-blue)
<br>

`libcc`是跨平台，多线程，轻量级的C语言库，提供了更简单的接口和更丰富的协议。提供的函数功能包括：字符串、日志、双向/单向链表、哈希表、网络通信、JSON、XML、INI配置文件读取、AES、DES、MD2、MD4、MD5、base16/base58/base64编码/解码、url编码/解码、时间轮计时器等等。详细信息请参阅C文件
```C
//跨平台，超轻量，易扩展，框架代码如下：
#define CALL(fn, ...) fn(__VA_ARGS__)

//使用示例：输出hello World!
CALL(printf, "Hello World!\n");
/*
 * 开一个玩笑:)！
 * 
 * 只是出于对C的热爱，累积一份属于自己的C代码
 */
```

## ✨ 特征

- 跨平台（Linux, Windows, MacOS, FreeBSD, Android, iOS）
- 高性能事件循环（网络IO事件、定时器事件）
- TCP/UDP服务端/客户端/代理
- TCP支持心跳、转发、拆包、多线程安全write和close等特性

## ⌛️ 安装教程

见[REANDME.Makefile](REANDME.Makefile)

libcc提供了以下构建方式:

```shell
#1、通过Makefile:编译 (Linux,freeBSD,macOS)
cd yourdirname/CC/build
make path

./build.sh debug
# Windows 下MSYS2环境 执行
./build.cmd debug

#2、通过Visual Studio编译 (Windows)
proj.Win/libcc.vcxproj

#3、通过Android JNI编译
#打开 .bash_profile 配置 $NDK_ROOT = (Android NDK目录)
cd proj.Android/JNI
./build_NDK.sh

#4、通过Xcode 编译(macOS,iOS)
proj.OSX/cc.xcodeproj
proj.IOS/cc.xcodeproj

#5、ubuntu 编译
sudo apt-get install libmysqlclient-dev
sudo apt-get install sqlite3
sudo apt-get install libsqlite3-dev 
sudo apt-get install libjpeg-dev
sudo apt-get install libpng-dev

#6、FreeBSD 安装

```

## ⚡️ 入门与体验

```shell
# 下载编译
git clone https://github.com/huai2001/CC.git
cd CC/build
make path
./build.sh
```
#### Support
Email: [libcc.cn@gmail.com](mailto:libcc.cn@gmail.com)

## ✨ TCP Client
```C

```
## ✨ TCP Server
```C


```
功能函数
------
