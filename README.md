CocosVR
================

This repository is a sample game showing how to run a cocos 3D game on VR platforms. The sample game is using cocos2d-x v3.10.

- Copyright: Chukong Technologies
- Source Code License: MIT
- Artwork License: You may not use any artworks in this sample game for commercial purpose

![](Screenshot.png)

##Git user attention

1.Clone the repo from Github

```
$ git clone https://github.com/chukong/CocosVR.git
```

2.Update the submodule of cocosVR

```
$ git submodule update --init
```

3.Update the submodule of cocos2d-x

```
$ cd cocos2d
$ git submodule update --init
```

4.After cloning the repo, please execute `download-deps.py` to download and install dependencies

```
$ cd cocos2d
$ python download-deps.py
```


##VR Platform support
This project supports Three VR platforms: **Oculus VR**(win32), **Gear VR** and **Deepoon VR**(Samsung Note4/5 S6).

###SDK Requires
OculusVR PC **SDK V0.8**.
>Requires compile the SDK source code by VS2015 and build **/MD(release) and /MDd(debug) Runtime Library**

OculusVR Mobile **SDK V1.0.0.1**.

Deepoon Mobile **SDK V0.1.2**.

###Environment Variables
**OVRSDKROOT:**  PATH/

**OVRSDKMOBILEROOT:** PATH/

**DEEPOONSDKROOT:** PATH/samples/samples/native/SDKLib/

> PATH means the absolute path of SDK root directory

###win32 runtime
Path: `cocosVR/proj.win32/cocosVR.sln`.
>Requires Visual Studio 2015 and above.

###android runtime
Path: `cocosVR/proj.android/`.

execute `build_native.py -V gearvr` for **Gear VR platforms**

execute `build_native.py -V deepoon` for **Deepoon VR platforms**

##Credits
* Game Effects: SongCheng Jiang
* Game Logic: SongCheng Jiang
* Game Sound: SongCheng Jiang
* VR technical support: SongCheng Jiang, HuaBing Xu, etc
