cmake_minimum_required(VERSION 2.6)
project(swmm_bin_read)

#add_definitions(-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED EXACT 2.7.3)
find_package(NumPy REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH} ${NUMPY_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS "-std=c99")
add_library(swmm_bin_read SHARED datetime.c reader.c enums.h)

#set(CMAKE_C_FLAGS "")

set_source_files_properties(swmm_bin_read.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_module(pyswmm_bin_read python swmm_bin_read.i)
swig_link_libraries(pyswmm_bin_read ${PYTHON_LIBRARIES} swmm_bin_read)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(test_reader main.cpp)
target_link_libraries(test_reader swmm_bin_read)
