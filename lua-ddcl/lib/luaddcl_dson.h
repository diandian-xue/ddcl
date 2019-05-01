#pragma once

#include "ddcldson.h"
#include "luaddcl.h"

DDCLLUA int
lddcl_push_dson2lua (lua_State * L, ddcl_DsonBuffer * dson);

DDCLLUA ddcl_DsonBuffer *
lddcl_push_lua2dson (lua_State * L, ddcl_DsonBuffer * dson,
        int start, int end);
