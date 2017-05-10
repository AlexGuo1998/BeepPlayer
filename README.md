# BeepPlayer
A cross-platform program to play music through PC speaker.

Chinese README is [HERE](#chinese)  
中文说明在 [这里](#chinese)

## How to Build

Clone the repository or download ZIP, then

* On Windows

  Open beepplayer\beepplayer.sln with latest Visual Studio (VS 2017 for now), then select `Build`-`Build Solution`

* On Linux

  ```
  cd beepplayer/beepplayer/
  make
  ```
  If you want the program to run as root automatically (potentially dangerous), run
  ```
  make chmod
  ```
  This will change the owner of the program to root, and set SUID.

## How to Use

First of all, you need some `music score` file, which can be found in `beepplayer/beepplayer/demo/`. The instruction for writing `music score` file can be found [HERE](doc/music-writing-instruction-en.md).

Then, run the program with  `text music` file as the last parameter.

* If you use windows, you can drag the music file to the executable file.
* If you use Linux, run as root.

```
(sudo) ./beepplayer ./demo/somemusic.txt
```

## Bugs / TODOs

[Change log](doc/changelog.md)

* Deal with `^c` ()
* Add some color to the lyric
* Wave (to stdout) for those pc without speaker
* Multi-thread: split play and display

---
<a name=chinese></a>
# BeepPlayer（蜂鸣器播放器）
一款让你的电脑蜂鸣器播放音乐的跨平台软件。

## 生成方法

请先克隆源或者下载zip包，然后

* Windows

  使用最新版本的Visual Studio（当前为Visual Studio 2017），打开beepplayer\beepplayer.sln，选择`生成`-`生成解决方案`即可。

* Linux

  ```
  cd beepplayer/beepplayer
  make
  ```
  如果你希望在程序启动时自动获得root权限（有潜在风险），可以执行
  ```
  make chmod
  ```
  将会把程序的所有者变为root，并设置SUID标记。

## 使用方法

首先，你需要一些`乐谱`，示例可在`beepplayer/beepplayer/demo/`文件夹下找到。`乐谱`的编写规则可以在 [这里](doc/music-writing-instruction-cn.md) 找到

然后，运行程序，并把`乐谱`作为最后一个参数传入。
* 在Windows下，可以将乐谱拖放到exe文件上来执行；
* 在Linux下，请用root身份执行。
```
(sudo) ./beepplayer ./demo/somemusic.txt
```

## Bugs / TODOs

[版本更新记录](doc/changelog.md)

* 正确处理 `^c` 按键
* 歌词颜色
* 输出wave文件（到stdout），以便无蜂鸣器电脑使用
* 多线程：播放和显示分离