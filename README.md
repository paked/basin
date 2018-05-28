# Basin

game related base code.

## Build Instructions

### Windows

1. Download SDL2 the runtime devel "VC" files for SDL2, SDL_image, and SDL_ttf
2. Place the extracted folders in a common directory (I'm currently using C:\dev).
3. Open command prompt
4. Go to the source directory, create a new folder which you want to build out of.
5. Type `..\build.bat`. This will create a new visual studio 2017 project which you can open and continue as normal. If you are using different versions of SDL to me, OR have a different directory where you put the runtime files you will need to edit the paths in `build.bat`
6. Copy relevant library files into `pwd` for when you run the executable
