
add_executable(example_1 orin_demo1.cpp)
target_link_libraries(example_1 PUBLIC benchmark_main)

add_executable(example_2 orin_demo2.cpp)
target_link_libraries(example_2 PUBLIC benchmark_main)

add_executable(example_3 orin_demo3.cpp)
target_link_libraries(example_3 PUBLIC benchmark_main)

add_executable(example_4 orin_demo4.cpp)
target_link_libraries(example_4 PUBLIC benchmark_main)

# https://mp.weixin.qq.com/s/18TujefnTj-17GuNgmYW-Q