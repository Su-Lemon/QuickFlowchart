cmake_minimum_required(VERSION 3.20)
project(QuickFlowchart)

set(CMAKE_CXX_STANDARD 17)

# 添加头文件的包含文件夹
include_directories(tools src ./src/symbols ./src/utils)

# 添加源文件的根目录（main.cpp）
aux_source_directory(src DIR_SRCS)
# 添加各模块文件夹中的库
add_subdirectory(src/symbols)
add_subdirectory(src/utils)

# 编译主文件夹中的main.cpp形成可执行文件QuickFlowchart.o
add_executable(QuickFlowchart ${DIR_SRCS})
# 将各模块的库链接到可执行文件
target_link_libraries(QuickFlowchart symbols utils)
