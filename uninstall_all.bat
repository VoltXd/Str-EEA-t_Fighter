@echo off
echo "*********** Warning ***********"
echo "You are going to uninstall the project build and it's external components (SDL2 and OpenCV)."

:choice
set /P c=Proceed? (y/n)
if /I "%c%" EQU "y" goto :Yes
if /I "%c%" EQU "n" goto :No
goto :choice

:Yes
echo Remove ./build
RD /S /Q "./build"
echo Remove C:/lib_visual_cpp/SDL2
RD /S /Q "C:/lib_visual_cpp/SDL2"
echo Remove C:/lib_cpp/SDL2
RD /S /Q "C:/lib_cpp/SDL2"
echo Remove C:/lib_visual_cpp/opencv_4.7.0
RD /S /Q "C:/lib_visual_cpp/opencv_4.7.0"
echo Remove C:/lib_cpp/opencv_4.7.0
RD /S /Q "C:/lib_cpp/opencv_4.7.0"

:No