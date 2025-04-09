Build and run on windows:
cmake --build build --config Debug
start build/Debug/app.exe

Clean on windows:
rmdir /s /q build
del CMakeCache.txt
del cmake_install.cmake
mkdir build
cmake -S . -B build
cmake --build build