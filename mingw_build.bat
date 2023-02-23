@echo off

:: ______________Build Project______________ ::
cmake -B ./build -G "MinGW Makefiles"
cd ./build
make 
cd ..