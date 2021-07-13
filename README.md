# PLUXER
## Introduction
PULXER리듬액션 게임으로 리듬에 맞춰 몬스터와 전투하여 각 스테이지에서 높은 스코어를 달성하는 게임입니다.  
싱글 플레이와 2인 협동 멀티 플레이를 지원합니다.  
[![video](https://img.youtube.com/vi/nVqEqLcRbjc/sddefault.jpg)](https://www.youtube.com/watch?v=nVqEqLcRbjc)  
Copyright (c) 2021 Game-institute-1st-While-true  

hosEngine으로 만들어 졌습니다.  
[![hosEngine](https://user-images.githubusercontent.com/45713360/124558885-08cb9900-de76-11eb-92ae-b538626f056e.png)](https://github.com/Game-institute-1st-While-true/hosEngine)

## How to Build
### requires
>Visual Studio 2019  
>Windows SDK 10.0  
>Platform Toolset v142  
>MFC Toolset v142 

#### 1. clone this repository onto your local drive

#### 2. FBX SDK install. becuase License
> https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-2  
> download FBX SDK 2020.2 vs2019 and install  
> copy "FBX SDK Directory/2020.2/include" to "hosEngine/Extern/Inc"  
> copy "FBX SDK Directory/2020.2/lib/vs2019/x64/debug" to "hosEngine/Extern/Lib"  
> > change name libfbxsdk-md.lib -> libfbxsdk_x64D.lib  
> > change name libxml2-md.lib -> libxml2_x64D.lib  
> > change name zlib-md.lib -> zlib_x64D.lib  
>
> copy "FBX SDK Directory/2020.2/lib/vs2019/x64/release" to "hosEngine/Extern/Lib"  
> > change name libfbxsdk-md.lib -> libfbxsdk_x64R.lib  
> > change name libxml2-md.lib -> libxml2_x64R.lib  
> > change name zlib-md.lib -> zlib_x64R.lib  

#### 3. open Build/hosEngine.sln

#### 4. Build hosClient and hosServer
!hosServer is not working. because need login server and DBA(db middleware - ex)redis), DB

## Documentation
![howtoplay](https://github.com/Game-institute-1st-While-true/PLUXER/blob/main/Bin/PLUXER_how_to_play.png)
