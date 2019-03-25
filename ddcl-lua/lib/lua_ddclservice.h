#pragma once

#include "ddclservice.h"
#include "ddcldson.h"
#include "lua_ddcl.h"

#define LDDCL_CTX_K "lddcl_service_context"

typedef struct tag_Context{
    lua_State * L;
    ddcl_DsonBuffer * dson;
    ddcl_Service svr;
    int is_worker;
    int be_main;

    int unknow;
    int startfn;
    int callback;
    int session_map;
    int co_map;
}Context;

#define LDDCL_FIND_CTX \
Context * ctx = NULL; \
{ \
    lua_pushstring(L, LDDCL_CTX_K); \
    lua_rawget(L, LUA_REGISTRYINDEX); \
    if(lua_isnil(L, -1)){ \
        lua_pop(L, 1); \
        return luaL_error(L, "can not found context"); \
    }\
    ctx = lua_touserdata(L, -1); \
    lua_pop(L, 1); \
}


DDCLLUA int
lddcl_yield_for_session(
        lua_State * L, Context * ctx, ddcl_Session session);

DDCLLUA int
lddcl_set_newservice_hook(lua_CFunction f);

DDCLLUA ddcl_Service
lddcl_new_luaservice (lua_State * L,
        const char * path, const char * cpath,
        const char * script, const char * param);
