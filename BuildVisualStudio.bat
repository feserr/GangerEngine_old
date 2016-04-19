@echo off

mkdir build
cd build
set /p version=Which version of Visual Studio do you want to use: 
cmake -G "Visual Studio %version%" ..
