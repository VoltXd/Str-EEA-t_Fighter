@echo off
echo Remove ./build
RD /S /Q "./build"
echo Remove C:/lib_visual_cpp/SDL2
RD /S /Q "C:/lib_visual_cpp/SDL2"
echo Remove C:/lib_cpp/SDL2
RD /S /Q "C:/lib_cpp/SDL2"