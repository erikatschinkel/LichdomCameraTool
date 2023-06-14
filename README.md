# LichdomCameraTool
A tool for taking screenshots in Lichdom, based on [HattiWatti's Cinematic Tools](https://github.com/Hattiwatti/CinematicTools)

Currently for GOG version, may do others at some point later

### This project is powered by
[ImGui](https://github.com/ocornut/imgui) by Omar Cornut

[MinHook](https://github.com/TsudaKageyu/minhook) by Tsuda Kageyu

[inih](https://github.com/benhoyt/inih) by Ben Hoyt

[DirectXTK](https://github.com/Microsoft/DirectXTK) by Microsoft

### Features
- Free camera, HUD toggle, time stop
- Hotsampling
- Sun direction control, fog control
- Various postprocessing control via UI
- Unrestricted dev console and dev menu enabled by default

### Readme
- Insert toggles the camera
- Delete toggles the time stop
- Home toggles the HUD
- End toggles the UI
- Inject with your favorite injector

### Notes
 - Do not inject during a loading screen or while the screen is black when it first opens, it will crash upon initialization
 - There may be crashes, I think sometimes things are going wrong by doing some render related things in a thread other than the game's render thread
 - I may or may not continue to add things to this tool
 - There is quite a bit of code not being used in CryLich.h, as well as some class objects that are stored by the tool and not used, and I have kept all this because it could be helpful to someone else, or me, later.
 - For some reason the mouse cursor disappears, and none of my attempts to revive it thus far have succeeded

Special thanks of course to Xaviant for making Lichdom: Battlemage, which is really a pretty cool game, and fun to play.

