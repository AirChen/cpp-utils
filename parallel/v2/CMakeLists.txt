cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(thrad_benchmark_test)
set(CMAKE_CXX_STANDARD 14)
add_subdirectory(benchmark)

# 设置你的 benchmark 源文件
set(BENCHMARK_SRC bench_test.cpp)
add_executable(bench ${BENCHMARK_SRC})
target_link_libraries(bench benchmark benchmark_main pthread gtest_main)

add_subdirectory(mp_demos)