cmake_minimum_required(VERSION 2.8)
project(ddcl)

include(ddcl.cmake)
include(3rd/lua.cmake)
include(3rd/lua-cjson.cmake)
include(lddcl.cmake)


set(OUTPUT_PATH bin)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_PATH})

