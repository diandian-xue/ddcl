#define DDCL_CORE

#include "ddcldson.h"
#include "ddclmalloc.h"

#include <stdlib.h>
#include <memory.h>

struct tag_ddcl_DsonBuffer {
    size_t len;
    size_t cap;
    size_t iter;
};

DDCLAPI inline ddcl_DsonBuffer *
ddcl_new_dsonbuffer (size_t cap){
    if(cap < (8 + sizeof(ddcl_DsonBuffer))){
        cap = 8 + sizeof(ddcl_DsonBuffer);
    }
    ddcl_DsonBuffer * dson = ddcl_malloc(sizeof(ddcl_DsonBuffer) + cap);
    dson->len = sizeof(ddcl_DsonBuffer);
    dson->cap = cap;
    dson->iter = sizeof(ddcl_DsonBuffer);
    return dson;
}

DDCLAPI inline void
ddcl_free_dsonbuffer (ddcl_DsonBuffer * dson){
    ddcl_free(dson);
}


DDCLAPI inline ddcl_DsonBuffer *
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
        ddcl_DsonBuffer * new = ddcl_malloc(ncap);
        memcpy(new, dson, dson->len);
        ddcl_free(dson);
        new->cap = ncap;
        return new;
    }
    return dson;
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
    char * b = ((char *)dson) + dson->iter;
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
            v->u.dson = arr;
            dson->iter += 1 + arr->len;
        }
        break;
    case DDCL_DSON_PTR:
        dson->iter += 1 + sizeof(size_t) + sizeof(void *);
        v->size = *((size_t *)&(b[1]));
        v->u.ptr = *((void **)&(b[1 + sizeof(size_t)]));
        break;
    case DDCL_DSON_NULL:
        dson->iter += 1;
        break;
    default:
        return 0;
    }
    return 1;
}

DDCLAPI inline size_t
ddcl_dsonbuffer_len (ddcl_DsonBuffer * dson){
    return dson->len;
}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_clear_dsonbuffer (ddcl_DsonBuffer * dson){
    dson->len = sizeof(ddcl_DsonBuffer);
    return dson;
}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_integer (ddcl_DsonBuffer * dson, ddint64 num){
    dson = ddcl_check_expand_dsonbuffer(dson, 1 + sizeof(ddint64));
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_INTEGER;
    *((ddint64 *)&(b[1])) = num;
    dson->len += sizeof(ddint64) + 1;
    return dson;
}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_number (ddcl_DsonBuffer * dson, double num){
    dson = ddcl_check_expand_dsonbuffer(dson, 1 + sizeof(double));
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_NUMBER;
    *((double *)&(b[1])) = num;
    dson->len += sizeof(double) + 1;
    return dson;
}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_string (
        ddcl_DsonBuffer * dson, const char * str, size_t len){
    dson = ddcl_check_expand_dsonbuffer(dson, 1 + sizeof(size_t) + len);
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_STRING;
    *((size_t *)&(b[1])) = len;
    memcpy(b + (1 + sizeof(size_t)), str, len);
    dson->len += 1 + sizeof(size_t) + len;
    return dson;
}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_array (
        ddcl_DsonBuffer * dson, ddcl_DsonBuffer * array){
    size_t alen = 1 + array->len;
    dson = ddcl_check_expand_dsonbuffer(dson, alen);
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_ARRAY;
    b++;
    memcpy(b, array, array->len);
    dson->len += alen;
    return dson;
}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_map (
        ddcl_DsonBuffer * dson, ddcl_DsonBuffer * array){
    size_t alen = 1 + array->len;
    dson = ddcl_check_expand_dsonbuffer(dson, alen);
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_MAP;
    b++;
    memcpy(b, array, array->len);
    dson->len += alen;
    return dson;
}


DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_ptr (
        ddcl_DsonBuffer * dson, void * ptr, size_t sz){
    size_t alen = 1 + sizeof(size_t) + sizeof(void *);
    dson = ddcl_check_expand_dsonbuffer(dson, alen);
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_PTR;
    *((size_t *)&(b[1])) = sz;
    *((void **)&(b[1 + sizeof(size_t)])) = ptr;
    dson->len += alen;
    return dson;

}

DDCLAPI inline ddcl_DsonBuffer *
ddcl_push_dsonbuffer_null (ddcl_DsonBuffer * dson){
    dson = ddcl_check_expand_dsonbuffer(dson, 1);
    char * b = (char *)dson + dson->len;
    b[0] = DDCL_DSON_NULL;
    dson->len += 1;
    return dson;
}
