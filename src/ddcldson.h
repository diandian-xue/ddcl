#pragma once


#include "ddcl.h"

enum ddcl_DSON_TYPE {
    DDCL_DSON_INTEGER = 1,
    DDCL_DSON_NUMBER,
    DDCL_DSON_STRING,
    DDCL_DSON_ARRAY,
    DDCL_DSON_MAP,
};


typedef struct tag_ddcl_DsonBuffer ddcl_DsonBuffer;
typedef struct tag_ddcl_Dson {
    int type;
    size_t size;
    union {
        ddint64 integer;
        double number;
        char * string;
        ddcl_DsonBuffer * buffer;
    }u;
}ddcl_Dson;


DDCLAPI inline ddcl_DsonBuffer *
ddcl_new_dsonbuffer (size_t cap);

DDCLAPI inline void
ddcl_free_dsonbuffer (ddcl_DsonBuffer * buffer);

DDCLAPI inline size_t
ddcl_push_dsonbuffer_integer (ddcl_DsonBuffer * buffer, ddint64 num);

DDCLAPI inline size_t
ddcl_push_dsonbuffer_number (ddcl_DsonBuffer * buffer, double num);

DDCLAPI inline size_t
ddcl_push_dsonbuffer_string (
        ddcl_DsonBuffer * buffer, const char * str, size_t len);

DDCLAPI inline size_t
ddcl_push_dsonbuffer_array (
        ddcl_DsonBuffer * buffer, ddcl_DsonBuffer * array);

DDCLAPI inline size_t
ddcl_push_dsonbuffer_map (
        ddcl_DsonBuffer * buffer, ddcl_DsonBuffer * map);

DDCLAPI inline size_t
ddcl_check_expand_dsonbuffer (
        ddcl_DsonBuffer * buffer, size_t cap);

DDCLAPI inline int
ddcl_begin_dsonbuffer (ddcl_DsonBuffer * buffer);

DDCLAPI inline int
ddcl_next_dsonbuffer (ddcl_DsonBuffer * buffer, ddcl_Dson * v);

