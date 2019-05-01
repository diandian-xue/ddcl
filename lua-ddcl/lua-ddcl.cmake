
set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB LIB_SOURCES ${LIB_PATH}/*)
add_library(libluaddcl
    ${LIB_SOURCES}
    )

file(GLOB SOURCES ${SRC_PATH}/*)

add_executable(luaddcl_exc
    ${SOURCES}
    )


if(${BUILD_SHARED_LIBS})
    target_compile_definitions(libluaddcl PUBLIC DDCLLUA_EXPORTS)
    set_target_properties(libluaddcl PROPERTIES PREFIX "")
endif()

target_include_directories(libluaddcl PUBLIC ${LIB_PATH})
target_link_libraries(libluaddcl PUBLIC lualib)
target_link_libraries(libluaddcl PUBLIC ddcl)
set_target_properties(libluaddcl PROPERTIES OUTPUT_NAME libluaddcl)


target_link_libraries(luaddcl_exc PUBLIC libluaddcl)
target_link_libraries(luaddcl_exc PUBLIC lua-cjson)
set_target_properties(luaddcl_exc PROPERTIES OUTPUT_NAME luaddcl)

if(LINUX)
    set_target_properties(luaddcl_exc PROPERTIES COMPILE_FLAGS -std=gnu99)
endif()


