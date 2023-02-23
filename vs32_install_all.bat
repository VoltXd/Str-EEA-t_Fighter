@echo off

:: ______________Install SDL______________ ::
cd ./libraries/SDL

:: Build SDL for VS 2022 Win32
cmake -B ./build -G "Visual Studio 17 2022" -A Win32 
cmake --build ./build --config Release

:: Install SDL at "C:\lib_visual_cpp\SDL2"
RD /S /Q "C:\lib_visual_cpp\SDL2"
cmake --install ./build --prefix "C:\lib_visual_cpp\SDL2"

:: Remove SDL build
RD /S /Q "./build"
cd ../..

:: ______________Build Project______________ ::
cmake -B ./build -G "Visual Studio 17 2022" -A Win32