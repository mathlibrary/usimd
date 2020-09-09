set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# 查找当前目录下的所有源文件

# 并将名称保存到 DIR_TEST_SRCS 变量

aux_source_directory(. DIR_TEST_SRCS)

# 生成链接库

add_executable(Demo usimd_test1.c)

# 添加链接库

target_link_libraries(Demo usimd)