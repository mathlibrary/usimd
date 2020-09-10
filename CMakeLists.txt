cmake_minimum_required(VERSION 2.8.0)
project(helloworld)

include_directories(./ ./src)
aux_source_directory(. DIR_TEST_SRCS)
add_subdirectory(src)
# 生成链接库
add_executable(Demo usimd_test1.c)
# 添加链接库
target_link_libraries(Demo usimd)