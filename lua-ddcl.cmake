
set(SRC_PATH ${PROJECT_SOURCE_DIR}/lua-ddcl)

file(GLOB HEADERS ${SRC_PATH}/*.h)
file(GLOB SOURCES ${SRC_PATH}/*.c)

add_library(lua-ddcl
    ${SOURCES}
    ${HEADERS}
    )


if(${BUILD_SHARED_LIBS})
    target_compile_definitions(lua-ddcl PUBLIC DDCLLUA_EXPORTS)
    set_target_properties(lua-ddcl PROPERTIES OUTPUT_NAME lua-ddcl)
    set_target_properties(lua-ddcl PROPERTIES PREFIX "")
endif()

target_include_directories(lua-ddcl PUBLIC ${SRC_PATH})
target_link_libraries(lua-ddcl PUBLIC lualib)
target_link_libraries(lua-ddcl PUBLIC ddcl)
set_target_properties(lua-ddcl PROPERTIES PREFIX "")
set_target_properties(lua-ddcl PROPERTIES OUTPUT_NAME lddcl)

if(LINUX)
    set(DDCLLUA_COMPILE_FLAGS -std=gnu99)
    set_target_properties(lua-ddcl PROPERTIES COMPILE_FLAGS ${DDCLLUA_COMPILE_FLAGS})
endif()

if(APPLE)
    if(${BUILD_SHARED_LIBS})
        set_target_properties(lua-ddcl PROPERTIES SUFFIX ".so")
    endif()
endif()

