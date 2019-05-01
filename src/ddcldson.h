#pragma once


#include "ddcl.h"

enum ddcl_DSON_TYPE {
    DDCL_DSON_INTEGER = 1,
    DDCL_DSON_NUMBER,
    DDCL_DSON_STRING,
    DDCL_DSON_ARRAY,
    DDCL_DSON_MAP,
    DDCL_DSON_PTR,
    DDCL_DSON_NULL,
};


typedef struct tag_ddcl_DsonBuffer ddcl_DsonBuffer;
typedef struct tag_ddcl_Dson {
    int type;
    size_t size;
    union {
        ddint64 integer;
        double number;
        char * string;
        ddcl_DsonBuffer * dson;
        void * ptr;
    }u;
}ddcl_Dson;


DDCLAPI ddcl_DsonBuffer *
ddcl_new_dsonbuffer (size_t cap);

DDCLAPI void
ddcl_free_dsonbuffer (ddcl_DsonBuffer * dson);

DDCLAPI ddcl_DsonBuffer *
ddcl_check_expand_dsonbuffer (
        ddcl_DsonBuffer * dson, size_t cap);

DDCLAPI int
ddcl_begin_dsonbuffer (ddcl_DsonBuffer * dson);

DDCLAPI int
ddcl_next_dsonbuffer (ddcl_DsonBuffer * dson, ddcl_Dson * v);

DDCLAPI size_t
ddcl_dsonbuffer_len (ddcl_DsonBuffer * dson);

DDCLAPI ddcl_DsonBuffer *
ddcl_clear_dsonbuffer (ddcl_DsonBuffer * dson);

DDCLAPI ddcl_DsonBuffer *
ddcl_push_dsonbuffer_integer (ddcl_DsonBuffer * dson, ddint64 num);

DDCLAPI ddcl_DsonBuffer *
ddcl_push_dsonbuffer_number (ddcl_DsonBuffer * dson, double num);

DDCLAPI  ddcl_DsonBuffer *
ddcl_push_dsonbuffer_string (
        ddcl_DsonBuffer * dson, const char * str, size_t len);

DDCLAPI ddcl_DsonBuffer *
ddcl_push_dsonbuffer_array (
        ddcl_DsonBuffer * dson, ddcl_DsonBuffer * array);

DDCLAPI ddcl_DsonBuffer *
ddcl_push_dsonbuffer_map (
        ddcl_DsonBuffer * dson, ddcl_DsonBuffer * map);

DDCLAPI ddcl_DsonBuffer *
ddcl_push_dsonbuffer_ptr (
        ddcl_DsonBuffer * dson, void * ptr, size_t sz);

DDCLAPI ddcl_DsonBuffer *
ddcl_push_dsonbuffer_null (ddcl_DsonBuffer * dson);
