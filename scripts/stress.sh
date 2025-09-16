#!/usr/bin/env bash
# 运行 demo，观察输出与并发
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make -j
# 直接运行
./pc_test
