cmake_minimum_required(VERSION 2.8)

project(ddcl)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})


include(ddcl.cmake)
add_subdirectory(ddcl_lua/ ddcl_lua)

