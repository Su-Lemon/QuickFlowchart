cmake_minimum_required(VERSION 3.20)
project(QuickFlowchart)

set(CMAKE_CXX_STANDARD 17)

include_directories(./src/symbols ./src/utils)

aux_source_directory(src DIR_SRCS)
add_subdirectory(src/symbols)
add_subdirectory(src/utils)

add_executable(QuickFlowchart ${DIR_SRCS})
target_link_libraries(QuickFlowchart symbols utils)
