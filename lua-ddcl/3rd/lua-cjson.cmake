cmake_minimum_required(VERSION 2.8)

set(LUA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua/src)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-cjson)
set(SOURCES
    ${SRC_PATH}/lua_cjson.c
    ${SRC_PATH}/strbuf.c
    )

set(CJSON_PRIVATE_DEFINITIONS "")
option(USE_INTERNAL_FPCONV "Use internal strtod() / g_fmt() code for performance")
if(NOT USE_INTERNAL_FPCONV)
    # Use libc number conversion routines (strtod(), sprintf())
    set(FPCONV_SOURCES ${SRC_PATH}/fpconv.c)
else()
    # Use internal number conversion routines
    #add_definitions(-DUSE_INTERNAL_FPCONV)
    list(APPEND CJSON_PRIVATE_DEFINITIONS USE_INTERNAL_FPCONV)
    set(FPCONV_SOURCES ${SRC_PATH}/g_fmt.c ${SRC_PATH}/dtoa.c)

    include(TestBigEndian)
    TEST_BIG_ENDIAN(IEEE_BIG_ENDIAN)
    if(IEEE_BIG_ENDIAN)
        #add_definitions(-DIEEE_BIG_ENDIAN)
        list(APPEND CJSON_PRIVATE_DEFINITIONS IEEE_BIG_ENDIAN)
    endif()

    if(MULTIPLE_THREADS)
        set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
        find_package(Threads REQUIRED)
        if(NOT CMAKE_USE_PTHREADS_INIT)
            message(FATAL_ERROR
                    "Pthreads not found - required by MULTIPLE_THREADS option")
        endif()
        #add_definitions(-DMULTIPLE_THREADS)
        list(APPEND CJSON_PRIVATE_DEFINITIONS MULTIPLE_THREADS)
    endif()
endif()

add_library(lua-cjson ${SOURCES} ${FPCONV_SOURCES})
set_target_properties(lua-cjson PROPERTIES PREFIX "")
set_target_properties(lua-cjson PROPERTIES OUTPUT_NAME cjson)
target_link_libraries(lua-cjson PUBLIC lualib)
target_include_directories(lua-cjson PUBLIC ${SRC_PATH})

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(lua-cjson PUBLIC LUA_CJSON_EXPORTS)
endif()


# Handle platforms missing isinf() macro (Eg, some Solaris systems).
include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(isinf math.h HAVE_ISINF)
if(NOT HAVE_ISINF)
    #add_definitions(-DUSE_INTERNAL_ISINF)
    list(APPEND CJSON_PRIVATE_DEFINITIONS USE_INTERNAL_ISINF)
endif()

if(APPLE)
    set(CMAKE_SHARED_MODULE_CREATE_C_FLAGS
        "${CMAKE_SHARED_MODULE_CREATE_C_FLAGS} -undefined dynamic_lookup")
    if(${BUILD_SHARED_LIBS})
        set_target_properties(lua-cjson PROPERTIES SUFFIX ".so")
    endif()
endif()

if(WIN32)
    # Windows sprintf()/strtod() handle NaN/inf differently. Not supported.
    list(APPEND CJSON_PRIVATE_DEFINITIONS DISABLE_INVALID_NUMBERS)
endif()

if(MSVC)
    list(APPEND CJSON_PRIVATE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    list(APPEND CJSON_PRIVATE_DEFINITIONS inline=__inline)
    list(APPEND CJSON_PRIVATE_DEFINITIONS strncasecmp=_strnicmp)
endif()

target_compile_definitions(lua-cjson PRIVATE ${CJSON_PRIVATE_DEFINITIONS})
