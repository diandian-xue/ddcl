#define DDCL_CORE

#include "ddclarray.h"

#include <stdlib.h>
#include <memory.h>

struct tag_ddcl_Array {
    size_t size;
    size_t count;
    size_t cap;
    char * data;
};

DDCLAPI inline ddcl_Array *
ddcl_new_array (size_t sz, size_t cap){
    if(cap < 4){
        cap = 4;
    }
    if(sz < 1){
        sz = 1;
    }
    ddcl_Array * arr = malloc(sizeof(ddcl_Array));
    arr->size = sz;
    arr->count = 0;
    arr->cap = cap;
    arr->data = malloc(sz * cap);
    return arr;
}

DDCLAPI inline void
ddcl_free_array (ddcl_Array * arr){
    free(arr->data);
    free(arr);
}

DDCLAPI inline size_t
ddcl_expand_array (ddcl_Array * arr, size_t cap){
    if(cap < arr->cap){
        if(arr->cap >= 2048){
            cap = 2048;
        }else{
            cap = arr->cap;
        }
        arr->cap += cap;
    }

    char * data = arr->data;
    arr->data = malloc(arr->size * arr->cap);
    if(arr->count){
        memcpy(arr->data, data, arr->size * arr->count);
    }
    return arr->cap;
}

DDCLAPI inline int
ddcl_push_array (ddcl_Array * arr, const void * data){
    if(arr->count >= arr->cap){
        ddcl_expand_array(arr, 0);
    }
    size_t off = arr->size * arr->count;
    memcpy(arr->data + off, data, arr->size);
    arr->count ++;
    return arr->count - 1;
}

DDCLAPI inline void *
ddcl_back_array (ddcl_Array * arr){
    if(arr->count){
        return (void *)(arr->data + (arr->size * arr->count - 1));
    }
    return NULL;
}

DDCLAPI inline void *
ddcl_front_array (ddcl_Array * arr){
    if(arr->count){
        return (void *)(arr->data);
    }
    return NULL;
}

DDCLAPI inline void
ddcl_pop_back_array (ddcl_Array * arr){
    if(!arr->count){
        return;
    }
    arr->count --;
}

DDCLAPI inline void
ddcl_pop_front_array (ddcl_Array * arr){
    ddcl_remove_array(arr, 0);
}

DDCLAPI inline void
ddcl_remove_array (ddcl_Array * arr, size_t index){
    if(!arr->count){
        return;
    }
    arr->count --;
    if(!arr->count){
        return;
    }
    memcpy(arr->data, arr->data + (arr->size * index), arr->size * arr->count);
}

DDCLAPI inline void *
ddcl_get_array (ddcl_Array * arr, size_t index){
    if(index >= arr->count){
        return NULL;
    }
    return (void *)(arr->data + (arr->size * index));
}

DDCLAPI inline size_t
ddcl_count_array (ddcl_Array * arr){
    return arr->count;
}

DDCLAPI inline void
ddcl_sort_array (ddcl_Array * arr, int(*cmp)(const void *, const void *)){
    if(arr->count < 2){
        return;
    }
    qsort(arr->data, arr->count, arr->size, cmp);
}
