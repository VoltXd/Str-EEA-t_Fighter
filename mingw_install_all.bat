@echo off

:: ______________Install SDL______________ ::
cd ./libraries/SDL

:: Build SDL for MinGW
cmake -B ./build -G "MinGW Makefiles" 
cmake --build ./build --config Release

:: Install SDL at "C:\lib_visual_cpp\SDL2"
RD /S /Q "C:\lib_cpp\SDL2"
cmake --install ./build --prefix "C:\lib_cpp\SDL2"

:: Remove SDL build
RD /S /Q "./build"
cd ../..

:: ______________Install OpenCV______________ ::
cd ./libraries/opencv

:: Build OpenCV for MinGW
cmake -B ./build -G "MinGW Makefiles"  
cmake --build ./build --config Release

:: Install OpenCV at "C:\lib_cpp\opencv_4.7.0"
RD /S /Q "C:\lib_cpp\opencv_4.7.0"
cmake --install ./build --prefix "C:\lib_cpp\opencv_4.7.0"

:: Remove SDL build
RD /S /Q "./build"
cd ../..

:: ______________Build Project______________ ::
cmake -B ./build -G "MinGW Makefiles"
cd ./build
make 
cd ..