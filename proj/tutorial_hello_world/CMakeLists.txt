cmake_minimum_required(VERSION 2.8.7)

#initialize project
set(proj_name hello_world)
project(${proj_name})
set(src_files "")

#set paths
if(NOT proj_src_path)
	set(proj_src_path ${PROJECT_SOURCE_DIR})
endif(NOT proj_src_path)

if(NOT root_path)
	set(root_path ${proj_src_path}/../..)
	add_definitions(-DROOT_PATH=\"${root_path}\")
endif(NOT root_path)

#include Eigen
include_directories(${root_path}/ext/eigen)

#include hearder and source files in /src
file(GLOB_RECURSE src_cpp ${root_path}/src/*.cpp)
file(GLOB_RECURSE src_h ${root_path}/src/*.h)
list(APPEND src_files ${src_cpp} ${src_h})
source_group("src" FILES ${src_cpp} ${src_h})
include_directories(${root_path}/src)
	
#include hearder and source files in /proj/a*
file(GLOB_RECURSE proj_cpp ${proj_src_path}/*.cpp)
file(GLOB_RECURSE proj_h ${proj_src_path}/*.h)
list(APPEND src_files ${proj_cpp} ${proj_h})
source_group("proj" FILES ${proj_cpp} ${proj_h})
include_directories(${proj_src_path})

#set compiling flags
set(CMAKE_CXX_STANDARD 11)	#c++11
if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")	#c++11
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")	#turn off sign-compare warning
endif(UNIX)
if(WIN32)
	add_definitions(-D_DISABLE_EXTENDED_ALIGNED_STORAGE)	#fix compiling issue for VS2017
endif(WIN32)

#add executable
add_executable(${proj_name} ${src_files})
