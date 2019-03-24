#define DDCL_CORE

#include "ddcldson.h"
#include "ddclmap.h"
#include "ddclarray.h"

#include <stdlib.h>
#include <memory.h>

struct tag_ddcl_DsonBuffer {
    size_t cur;
    size_t len;
    size_t cap;
    size_t iter;
};

DDCLAPI inline ddcl_DsonBuffer *
ddcl_new_dsonbuffer (size_t cap){
    if(cap < 8){
        cap = 8;
    }
    ddcl_DsonBuffer * buffer = malloc(sizeof(ddcl_DsonBuffer) + cap);
    buffer->cur = 0;
    buffer->len = 0;
    buffer->cap = cap;
    buffer->iter = 0;
    return buffer;
}

DDCLAPI inline void
ddcl_free_dsonbuffer (ddcl_DsonBuffer * buffer){
    free(buffer);
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_integer (ddcl_DsonBuffer * buffer, ddint64 num){
    ddcl_check_expand_dsonbuffer(buffer, 1 + sizeof(ddint64));
    char * b = (char *)buffer + sizeof(ddcl_DsonBuffer) + buffer->len;
    b[0] = DDCL_DSON_INTEGER;
    *((ddint64 *)&(b[1])) = num;
    buffer->len += sizeof(ddint64) + 1;
    return buffer->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_number (ddcl_DsonBuffer * buffer, double num){
    ddcl_check_expand_dsonbuffer(buffer, 1 + sizeof(double));
    char * b = (char *)buffer + sizeof(ddcl_DsonBuffer) + buffer->len;
    b[0] = DDCL_DSON_NUMBER;
    *((double *)&(b[1])) = num;
    buffer->len += sizeof(double) + 1;
    return buffer->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_string (
        ddcl_DsonBuffer * buffer, const char * str, size_t len){
    ddcl_check_expand_dsonbuffer(buffer, 1 + sizeof(size_t) + len);
    char * b = (char *)buffer + sizeof(ddcl_DsonBuffer) + buffer->len;
    b[0] = DDCL_DSON_STRING;
    *((size_t *)&(b[1])) = len;
    memcpy(b + (1 + sizeof(size_t)), str, len);
    buffer->len += 1 + sizeof(size_t) + len;
    return buffer->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_array (
        ddcl_DsonBuffer * buffer, ddcl_DsonBuffer * array){
    size_t alen = 1 + sizeof(size_t) + array->len;
    ddcl_check_expand_dsonbuffer(buffer, alen);
    char * b = (char *)buffer + sizeof(ddcl_DsonBuffer) + buffer->len;
    b[0] = DDCL_DSON_ARRAY;
    *((size_t *)&(b[1])) = array->len;
    memcpy(b + (1 + sizeof(size_t)), array,
            sizeof(ddcl_DsonBuffer) + array->len);
    buffer->len += alen;
    return buffer->len;
}

DDCLAPI inline size_t
ddcl_push_dsonbuffer_map (
        ddcl_DsonBuffer * buffer, ddcl_DsonBuffer * map){
    size_t alen = 1 + sizeof(size_t) + sizeof(ddcl_DsonBuffer) + map->len;
    ddcl_check_expand_dsonbuffer(buffer, alen);
    char * b = (char *)buffer + sizeof(ddcl_DsonBuffer) + buffer->len;
    b[0] = DDCL_DSON_MAP;
    *((size_t *)&(b[1])) = map->len;
    memcpy(b + (1 + sizeof(size_t)), map,
            sizeof(ddcl_DsonBuffer) + map->len);
    buffer->len += alen;
    return buffer->len;
}


DDCLAPI inline size_t
ddcl_check_expand_dsonbuffer (
        ddcl_DsonBuffer * buffer, size_t cap){
    if(cap > (buffer->cap - buffer->len)){
        size_t ncap = buffer->cap;
        if(ncap > 2048){
            ncap = 2048;
        }
        if(ncap < cap){
            ncap = cap;
        }
        ncap = buffer->cap + ncap;
        buffer->cap = ncap;
    }
    return buffer->cap;
}

DDCLAPI inline int
ddcl_begin_dsonbuffer (ddcl_DsonBuffer * buffer){
    buffer->iter = 0;
    return 0;
}

DDCLAPI inline int
ddcl_next_dsonbuffer (ddcl_DsonBuffer * buffer, ddcl_Dson * v){
    if(buffer->iter >= buffer->len){
        return 0;
    }
    char * b = (char *)buffer + sizeof(ddcl_DsonBuffer);
    v->type = b[0];
    v->size = 0;
    switch(b[0]){
    case DDCL_DSON_INTEGER:
        buffer->iter += 1 + sizeof(ddint64);
        v->u.integer = *((ddint64 *)&(b[1]));
        break;
    case DDCL_DSON_NUMBER:
        buffer->iter += 1 + sizeof(double);
        v->u.number = *((double *)&(b[1]));
        break;
    case DDCL_DSON_STRING:
        v->size = *((size_t *)&(b[1]));
        buffer->iter += 1 + sizeof(size_t) + v->size;
        v->u.string =  b + 1 + sizeof(size_t);
        break;
    case DDCL_DSON_ARRAY:
        break;
    }

    return 1;
}
