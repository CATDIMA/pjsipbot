#!/bin/bash

#get submodule
git submodule update --init

#build the pjproject submodule
cd dep/pjsip
./configure CFLAGS="-fPIC"
make dep
make
cd ../..

#build the bot
mkdir build
cp res build/res
cd build
mkdir records
cmake ..
cmake --build .