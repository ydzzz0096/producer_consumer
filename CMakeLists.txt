cmake_minimum_required(VERSION 3.10)
project(producer_consumer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加线程库，确保多线程代码可以正确编译和链接
find_package(Threads REQUIRED)

# --- 原有的测试程序编译规则 ---
add_executable(pc_test tests/test_queue.cpp)

# 设置头文件包含路径，让编译器知道去哪里找 "threadsafe_queue.h"
target_include_directories(pc_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 链接线程库
target_link_libraries(pc_test PRIVATE Threads::Threads)


# --- 【新增】主功能程序的编译规则 ---
# 1. 添加一个新的可执行文件目标，名为 pc_demo，源文件是 main.cpp
add_executable(pc_demo src/main.cpp)

# 2. 为 pc_demo 设置头文件包含路径
target_include_directories(pc_demo PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 3. 为 pc_demo 链接线程库
target_link_libraries(pc_demo PRIVATE Threads::Threads)