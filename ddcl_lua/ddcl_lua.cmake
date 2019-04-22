
set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB LIB_SOURCES ${LIB_PATH}/*)
add_library(ddcllua_lib
    ${LIB_SOURCES}
    )

file(GLOB SOURCES ${SRC_PATH}/*)

add_executable(ddcllua_excutable
    ${SOURCES}
    )


if(${BUILD_SHARED_LIBS})
    target_compile_definitions(ddcllua_lib PUBLIC DDCLLUA_EXPORTS)
    set_target_properties(ddcllua_lib PROPERTIES OUTPUT_NAME ddcllua)
    set_target_properties(ddcllua_lib PROPERTIES PREFIX "")
endif()

target_include_directories(ddcllua_lib PUBLIC ${LIB_PATH})
target_link_libraries(ddcllua_lib PUBLIC lualib)
target_link_libraries(ddcllua_lib PUBLIC ddcl)


target_link_libraries(ddcllua_excutable PUBLIC ddcllua_lib)
set_target_properties(ddcllua_excutable PROPERTIES OUTPUT_NAME ddcllua)

if(LINUX)
    set_target_properties(ddcllua_excutable PROPERTIES COMPILE_FLAGS -std=gnu99)
endif()


