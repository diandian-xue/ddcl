#pragma once


#include "ddcl.h"


typedef struct tag_ddcl_Array ddcl_Array;

DDCLAPI inline ddcl_Array *
ddcl_new_array (size_t sz, size_t cap);

DDCLAPI inline void
ddcl_free_array (ddcl_Array *);

DDCLAPI inline size_t
ddcl_expand_array (ddcl_Array *, size_t cap);

DDCLAPI inline int
ddcl_push_array (ddcl_Array *, const void * data);

DDCLAPI inline void *
ddcl_back_array (ddcl_Array *);

DDCLAPI inline void *
ddcl_front_array (ddcl_Array *);

DDCLAPI inline void
ddcl_pop_back_array (ddcl_Array *);

DDCLAPI inline void
ddcl_pop_front_array (ddcl_Array *);

DDCLAPI inline void
ddcl_remove_array (ddcl_Array *, size_t index);

DDCLAPI inline void *
ddcl_get_array (ddcl_Array *, size_t index);

DDCLAPI inline size_t
ddcl_count_array (ddcl_Array *);

DDCLAPI inline void
ddcl_sort_array (ddcl_Array *, int(*cmp)(const void *, const void *));
