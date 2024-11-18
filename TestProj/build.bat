@echo off

pushd src

clang -g -O0 main.cpp more/more.cpp more/hash.cpp -o ../main.exe -luser32 -lgdi32 


popd