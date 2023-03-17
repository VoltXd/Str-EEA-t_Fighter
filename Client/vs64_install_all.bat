@echo off

set install_path=:\lib_visual_cpp

:: ______________Choosing disk______________ ::

:: List availables disks
set disks=

:: Loop on all possible disks
SETLOCAL EnableDelayedExpansion
for %%a in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call :foo %%a 
goto end

:foo
if exist %1:\nul (
    :: If the disk exists, add it to the disks string
    set disks=!disks!%1 
)
:: Return to the for loop
goto :eof

:: End of for loop
:end

:: Ask the user to select a disk
:choice
set /P disk=Select a disk (%disks%)
call set disks_without_selection=%%disks:%disk%=%%
if not x%disks_without_selection%==x%disks% (
    :: The disk is in the available disks string => OK
    goto :OK
) else (
    :: Else => retry
    goto :choice
)

:OK

:: Set the install path
set install_path=%disk%%install_path%

:: ______________Install SDL______________ ::
cd ../libraries/SDL

:: Build SDL for VS 2022 x64
cmake -B ./build -G "Visual Studio 17 2022" -A x64  
cmake --build ./build --config Release

:: Install SDL at "C:\lib_visual_cpp\SDL2"
RD /S /Q "%install_path%\SDL2"
cmake --install ./build --prefix "%install_path%\SDL2"

:: Remove SDL build
RD /S /Q "./build"
cd ../..

:: ______________Install OpenCV______________ ::
cd ./libraries/opencv

:: Build OpenCV for VS 2022 x64
cmake -B ./build -G "Visual Studio 17 2022" -A x64  
cmake --build ./build --config Release

:: Install OpenCV at "C:\lib_visual_cpp\opencv_4.7.0"
RD /S /Q "%install_path%\opencv_4.7.0"
cmake --install ./build --prefix "%install_path%\opencv_4.7.0"

:: Remove OpenCV build
RD /S /Q "./build"
cd ../..

:: ______________Install LibRealSense______________ ::
cd ./libraries/librealsense

:: Build LibRealSense for VS 2022 x64
cmake -B ./build -G "Visual Studio 17 2022" -A x64  
cmake --build ./build --config Release

:: Install LibRealSense at "C:\lib_visual_cpp\realsense2_v2.53.1"
RD /S /Q "%install_path%\realsense2_v2.53.1"
cmake --install ./build --prefix "%install_path%\realsense2_v2.53.1"

:: Remove LibRealSense build
RD /S /Q "./build"
cd ../..

:: ______________Install SDL_image______________ ::
cd ./libraries/SDL_image

:: Build SDL_image for VS 2022 x64
cmake -B ./build -G "Visual Studio 17 2022" -A x64  
cmake --build ./build --config Release

:: Install SDL_image at "C:\lib_visual_cpp\SDL2_image"
RD /S /Q "%install_path%\SDL2_image"
cmake --install ./build --prefix "%install_path%\SDL2_image"

:: Remove SDL_image build
RD /S /Q "./build"
cd ../..

:: ______________Install SDL_mixer______________ ::
cd ./libraries/SDL_mixer

:: Build SDL_mixer for VS 2022 x64
cmake -B ./build -G "Visual Studio 17 2022" -A x64  
cmake --build ./build --config Release

:: Install SDL_mixer at "C:\lib_visual_cpp\SDL2_mixer"
RD /S /Q "%install_path%\SDL2_mixer"
cmake --install ./build --prefix "%install_path%\SDL2_mixer"

:: Remove SDL_mixer build
RD /S /Q "./build"
cd ../..

:: ______________Install SDL_ttf______________ ::
cd ./libraries/SDL_ttf

:: Build SDL_ttf for VS 2022 x64
cmake -B ./build -G "Visual Studio 17 2022" -A x64  
cmake --build ./build --config Release

:: Install SDL_ttf at "C:\lib_visual_cpp\SDL2_ttf"
RD /S /Q "%install_path%\SDL2_ttf"
cmake --install ./build --prefix "%install_path%\SDL2_ttf"

:: Remove SDL_ttf build
RD /S /Q "./build"
cd ../..

:: ______________Build Project______________ ::
cd ./Client
cmake -B ./build -G "Visual Studio 17 2022" -A x64

pause