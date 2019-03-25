
set(LIB_PATH ${PROJECT_SOURCE_DIR}/ddcl-lua/lib)
set(SRC_PATH ${PROJECT_SOURCE_DIR}/ddcl-lua/src)

file(GLOB LIB_SOURCES ${LIB_PATH}/*)
add_library(ddcl-lua_lib
    ${LIB_SOURCES}
    )

file(GLOB SOURCES ${SRC_PATH}/*)

add_executable(ddcl-lua
    ${SOURCES}
    )


if(${BUILD_SHARED_LIBS})
    target_compile_definitions(ddcl-lua_lib PUBLIC DDCLLUA_EXPORTS)
    set_target_properties(ddcl-lua_lib PROPERTIES OUTPUT_NAME ddcllua)
    set_target_properties(ddcl-lua_lib PROPERTIES PREFIX "")
endif()

target_include_directories(ddcl-lua_lib PUBLIC ${LIB_PATH})
target_link_libraries(ddcl-lua_lib PUBLIC lualib)
target_link_libraries(ddcl-lua_lib PUBLIC ddcl)


target_link_libraries(ddcl-lua PUBLIC ddcl-lua_lib)
set_target_properties(ddcl-lua PROPERTIES OUTPUT_NAME ddcllua)

if(LINUX)
    set_target_properties(ddcl PROPERTIES COMPILE_FLAGS -std=gnu99)
endif()


