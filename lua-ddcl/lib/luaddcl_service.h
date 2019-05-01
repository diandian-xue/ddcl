#pragma once

#include "ddclservice.h"
#include "ddcldson.h"
#include "luaddcl.h"

extern int LDDCL_CTX_K; //"luaddcl_service_context"

typedef struct tag_Context{
    lua_State * L;
    ddcl_DsonBuffer * dson;
    ddcl_Service svr;
    int is_worker;
    int be_main;

    int unknow;
    int startfn;
    int callback;

    // [session] = lua_State or lua_Function
    int session_map;

    // [lua_State] = {source, session}
    int co_map;
}Context;

#define LDDCL_FIND_CTX \
Context * ctx = NULL; \
{ \
    lua_rawgetp(L, LUA_REGISTRYINDEX, &LDDCL_CTX_K); \
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
        const char * path[], const char * cpath[],
        const char * script, ddcl_DsonBuffer * dson,
        ddcl_Service from, ddcl_Session session);
