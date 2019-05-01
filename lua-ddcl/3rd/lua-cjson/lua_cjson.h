#pragma once
#include <lua.h>
#include <lauxlib.h>

#if defined(_WIN32) || defined(_WIN64)
    #ifdef LUA_CJSON_EXPORTS
        #define LUA_CJSON_API    __declspec(dllexport)
    #else
        #define LUA_CJSON_API
    #endif
#else
    #define LUA_CJSON_API
#endif

LUA_CJSON_API int luaopen_cjson(lua_State *l);

LUA_CJSON_API int luaopen_cjson_safe(lua_State *l);
