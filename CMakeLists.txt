cmake_minimum_required(VERSION 3.10)
project(producer_consumer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

add_executable(pc_demo src/main.cpp)
add_executable(pc_test tests/test_queue.cpp)
target_include_directories(pc_demo PRIVATE src)
target_include_directories(pc_test PRIVATE src)
