#   Copyright (C) 2011-2013  Povilas Kanapickas <povilas@radix.lt>
#
#   Distributed under the Boost Software License, Version 1.0.
#       (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 2.8.0)
project(usimd)
set(PROJECT_VERSION "1.0")
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 添加 cpuinfo 子目录
add_subdirectory(cpuinfo)
# 生成链接库
add_library (usimd ${DIR_SRCS})
# 添加链接库
target_link_libraries(usimd cpuinfo)