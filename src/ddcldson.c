#define DDCL_CORE

#include "ddcldson.h"
#include "ddclmalloc.h"

#include <stdlib.h>
#include <memory.h>

struct tag_ddcl_DsonBuffer {
    size_t len;
    size_t cap;
    size_t iter;
    char * buffer;
};

DDCLAPI inline ddcl_DsonBuffer *
ddcl_new_dsonbuffer (size_t cap){
    if(cap < (8 + sizeof(ddcl_DsonBuffer))){
        cap = 8 + sizeof(ddcl_DsonBuffer);
    }
    ddcl_DsonBuffer * dson = ddcl_malloc(sizeof(ddcl_DsonBuffer));
    dson->buffer = ddcl_malloc(cap);
    dson->len = sizeof(ddcl_DsonBuffer);
    dson->cap = cap;
    dson->iter = 0;
    return dson;
}

DDCLAPI inline void
ddcl_free_dsonbuffer (ddcl_DsonBuffer * dson){
    ddcl_free(dson->buffer);
    ddcl_free(dson);
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_integer (ddcl_DsonBuffer * dson, ddint64 num){
    ddcl_check_expand_dsonbuffer(dson, 1 + sizeof(ddint64));
    char * b = dson->buffer + dson->len;
    b[0] = DDCL_DSON_INTEGER;
    *((ddint64 *)&(b[1])) = num;
    dson->len += sizeof(ddint64) + 1;
    return dson->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_number (ddcl_DsonBuffer * dson, double num){
    ddcl_check_expand_dsonbuffer(dson, 1 + sizeof(double));
    char * b = dson->buffer + dson->len;
    b[0] = DDCL_DSON_NUMBER;
    *((double *)&(b[1])) = num;
    dson->len += sizeof(double) + 1;
    return dson->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_string (
        ddcl_DsonBuffer * dson, const char * str, size_t len){
    ddcl_check_expand_dsonbuffer(dson, 1 + sizeof(size_t) + len);
    char * b = dson->buffer + dson->len;
    b[0] = DDCL_DSON_STRING;
    *((size_t *)&(b[1])) = len;
    memcpy(b + (1 + sizeof(size_t)), str, len);
    dson->len += 1 + sizeof(size_t) + len;
    return dson->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_array (
        ddcl_DsonBuffer * dson, ddcl_DsonBuffer * array){
    size_t alen = 1 + sizeof(ddcl_DsonBuffer) + array->len;
    ddcl_check_expand_dsonbuffer(dson, alen);
    char * b = dson->buffer + dson->len;
    b[0] = DDCL_DSON_ARRAY;
    ddcl_DsonBuffer * copy = (ddcl_DsonBuffer *)(&b[1]);
    *copy = *array;

    b += 1 + sizeof(ddcl_DsonBuffer);
    memcpy(b, array->buffer, array->len);
    dson->len += alen;
    return dson->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_map (
        ddcl_DsonBuffer * dson, ddcl_DsonBuffer * array){
    size_t alen = 1 + sizeof(ddcl_DsonBuffer) + array->len;
    ddcl_check_expand_dsonbuffer(dson, alen);
    char * b = dson->buffer + dson->len;
    b[0] = DDCL_DSON_MAP;
    ddcl_DsonBuffer * copy = (ddcl_DsonBuffer *)(&b[1]);
    *copy = *array;

    b += 1 + sizeof(ddcl_DsonBuffer);
    memcpy(b, array->buffer, array->len);
    dson->len += alen;
    return dson->len;
}


DDCLAPI inline size_t
ddcl_check_expand_dsonbuffer (
        ddcl_DsonBuffer * dson, size_t cap){
    if(cap > (dson->cap - dson->len)){
        size_t ncap = dson->cap;
        if(ncap > 2048){
            ncap = 2048;
        }
        if(ncap < cap){
            ncap = cap;
        }
        ncap = dson->cap + ncap;
        dson->cap = ncap;
        char * b = ddcl_malloc(ncap);
        memcpy(b, dson->buffer, dson->len);
        ddcl_free(dson->buffer);
        dson->buffer = b;
    }
    return dson->cap;
}

DDCLAPI inline int
ddcl_begin_dsonbuffer (ddcl_DsonBuffer * dson){
    dson->iter = sizeof(ddcl_DsonBuffer);
    return 0;
}

DDCLAPI inline int
ddcl_next_dsonbuffer (ddcl_DsonBuffer * dson, ddcl_Dson * v){
    if(dson->iter >= dson->len){
        return 0;
    }
    char * b = dson->buffer + dson->iter;
    v->type = b[0];
    v->size = 0;
    switch(b[0]){
    case DDCL_DSON_INTEGER:
        dson->iter += 1 + sizeof(ddint64);
        v->u.integer = *((ddint64 *)&(b[1]));
        break;
    case DDCL_DSON_NUMBER:
        dson->iter += 1 + sizeof(double);
        v->u.number = *((double *)&(b[1]));
        break;
    case DDCL_DSON_STRING:
        v->size = *((size_t *)&(b[1]));
        dson->iter += 1 + sizeof(size_t) + v->size;
        v->u.string =  b + 1 + sizeof(size_t);
        break;
    case DDCL_DSON_ARRAY:
    case DDCL_DSON_MAP:
        {
            ddcl_DsonBuffer * arr = (ddcl_DsonBuffer *)(b + 1);
            v->u.dson = arr;
            v->u.dson->buffer = b + 1 + sizeof(ddcl_DsonBuffer);
            dson->iter += 1 + sizeof(ddcl_DsonBuffer) + arr->len;
        }
        break;
    default:
        return 0;
    }
    return 1;
}

DDCLAPI inline char *
ddcl_dsonbuffer_buffer (ddcl_DsonBuffer * dson, size_t * len){
    if(len){
        *len = dson->len;
    }
    ddcl_DsonBuffer * head = (ddcl_DsonBuffer *)dson->buffer;
    *head = *dson;
    head->buffer = dson->buffer;
    return dson->buffer;
}

DDCLAPI inline void
ddcl_clear_dsonbuffer (ddcl_DsonBuffer * dson){
    dson->len = sizeof(ddcl_DsonBuffer);
}
