#!/bin/bash
rm -rf build/*
mkdir -p build
cd build
cmake ..
make
cd ..
mkdir -p bin
mv ./build/fastcam ./bin

