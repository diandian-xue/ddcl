﻿#define DDCL_CORE

#include "ddclmap.h"

#include <stdlib.h>

#ifdef DDMALLOC_RECORD
    #define ddcl_malloc(sz) ddcl_mallocR(sz, __FILE__, __LINE__)
    #define ddcl_free(p) ddcl_freeR(p, __FILE__, __LINE__)

    #define ddcl_mallocfn       ddcl_mallocR
    #define ddcl_freefn     ddcl_freeR

#else
    #define ddcl_malloc(sz) malloc(sz)
    #define ddcl_free(p) free(p)

    #define ddcl_mallocfn       malloc
    #define ddcl_freefn     free

#endif

DDCLAPI int
ddcl_init_malloc_module ();

DDCLAPI void
ddcl_exit_malloc_module ();

DDCLAPI void *
ddcl_mallocR(size_t sz, char * r, int line);

DDCLAPI void
ddcl_freeR(void * p, char * r, int line);

DDCLAPI void *
ddcl_realloc(void * p, size_t sz);

DDCLAPI void
ddcl_print_malloc_info ();
