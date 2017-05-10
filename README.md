# BeepPlayer
A cross-platform program to play music through PC speaker.

Chinese README is [HERE](#chinese)  
����˵���� [����](#chinese)

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
# BeepPlayer����������������
һ������ĵ��Է������������ֵĿ�ƽ̨�����

## ���ɷ���

���ȿ�¡Դ��������zip����Ȼ��

* Windows

  ʹ�����°汾��Visual Studio����ǰΪVisual Studio 2017������beepplayer\beepplayer.sln��ѡ��`����`-`���ɽ������`���ɡ�

* Linux

  ```
  cd beepplayer/beepplayer
  make
  ```
  �����ϣ���ڳ�������ʱ�Զ����rootȨ�ޣ���Ǳ�ڷ��գ�������ִ��
  ```
  make chmod
  ```
  ����ѳ���������߱�Ϊroot��������SUID��ǡ�

## ʹ�÷���

���ȣ�����ҪһЩ`����`��ʾ������`beepplayer/beepplayer/demo/`�ļ������ҵ���`����`�ı�д��������� [����](doc/music-writing-instruction-cn.md) �ҵ�

Ȼ�����г��򣬲���`����`��Ϊ���һ���������롣
* ��Windows�£����Խ������Ϸŵ�exe�ļ�����ִ�У�
* ��Linux�£�����root���ִ�С�
```
(sudo) ./beepplayer ./demo/somemusic.txt
```

## Bugs / TODOs

[�汾���¼�¼](doc/changelog.md)

* ��ȷ���� `^c` ����
* �����ɫ
* ���wave�ļ�����stdout�����Ա��޷���������ʹ��
* ���̣߳����ź���ʾ����