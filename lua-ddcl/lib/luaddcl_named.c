#define DDCLLUA_CORE

#include "ddclmap.h"
#include "ddclthread.h"
#include "ddclservice.h"
#include "luaddcl.h"

#include "string.h"

static ddcl_Map * _map = NULL;
static ddcl_RWLock _lock;

static int
l_named (lua_State * L){
    size_t len;
    const char * name = luaL_checklstring(L, 1, &len);
    ddcl_Service h = luaL_checkinteger(L, 2);
    ddcl_wlock_rw(&_lock);
    ddcl_set_map(_map, name, len, &h, sizeof(h));
    ddcl_wunlock_rw(&_lock);
    return 0;
}

static int
l_get_named (lua_State * L){
    size_t len;
    const char * name = luaL_checklstring(L, 1, &len);
    ddcl_rlock_rw(&_lock);
    ddcl_Service * h = ddcl_get_map(_map, name, len, NULL);
    ddcl_runlock_rw(&_lock);
    if(!h){
        return 0;
    }
    lua_pushinteger(L, *h);
    return 1;
}

static int
l_del_named (lua_State * L){
    size_t len;
    const char * name = luaL_checklstring(L, 1, &len);
    ddcl_wlock_rw(&_lock);
    ddcl_set_map(_map, name, len, NULL, 0);
    ddcl_wunlock_rw(&_lock);
    return 0;
}


int
openlib_named (lua_State * L){
    if(!_map){
        _map = ddcl_new_map(NULL, NULL);
        ddcl_init_rw(&_lock);
    }

    DDLUA_PUSHFUNC(L, "named", l_named);
    DDLUA_PUSHFUNC(L, "get_named", l_get_named);
    DDLUA_PUSHFUNC(L, "del_named", l_del_named);

    return 0;
}
