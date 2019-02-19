cmake_minimum_required(VERSION 2.8.7)

#initialize project
set(proj_name tutorial_spmtx)
project(${proj_name})
set(src_files main.cpp)

#include Eigen
include_directories(${PROJECT_SOURCE_DIR}/../../ext/eigen)

if(NOT proj_path)
	set(proj_src_path ${PROJECT_SOURCE_DIR})
endif(NOT proj_path)

if(NOT root_path)
	set(root_path ${proj_src_path}/../..)
endif(NOT root_path)

#include hearder and source files in /src
file(GLOB_RECURSE src_cpp ${root_path}/src/*.cpp)
file(GLOB_RECURSE src_h ${root_path}/src/*.h)
list(APPEND src_files ${src_cpp} ${src_h})
source_group("src" FILES ${src_cpp} ${src_h})
include_directories(${root_path}/src)


#add executable
add_executable(${proj_name} ${src_files})