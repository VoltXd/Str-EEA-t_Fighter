@echo off
echo "*********** Warning ***********"
echo "You are going to uninstall the project build and it's external components (SDL2 and OpenCV)."

:choice
set /P c=Proceed? (y/n)
if /I "%c%" EQU "y" goto :Yes
if /I "%c%" EQU "n" goto :No
goto :choice

:Yes

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

echo Remove ./build
RD /S /Q "./build"
echo Remove %disk%:/lib_visual_cpp/SDL2
RD /S /Q "%disk%:/lib_visual_cpp/SDL2"
echo Remove %disk%:/lib_cpp/SDL2
RD /S /Q "%disk%:/lib_cpp/SDL2"
echo Remove %disk%:/lib_visual_cpp/opencv_4.7.0
RD /S /Q "%disk%:/lib_visual_cpp/opencv_4.7.0"
echo Remove %disk%:/lib_cpp/opencv_4.7.0
RD /S /Q "%disk%:/lib_cpp/opencv_4.7.0"

pause

:No