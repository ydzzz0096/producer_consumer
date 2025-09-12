#!/usr/bin/env bash
mkdir -p build
cd build
cmake .. && make -j
./pc_demo
