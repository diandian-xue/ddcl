#pragma once

#include "ddclconfig.h"
#include "ddcl.h"

#ifdef DDMALLOC_RECORD
    #define ddcl_register_in_storage(HS, P) \
        ddcl_register_in_storage_record(HS, P, __FILE__, __LINE__)
    #define ddcl_new_storage(E, C) \
        ddcl_new_storage_record(E, C, __FILE__, __LINE__)
#else
    #define ddcl_register_in_storage ddcl_register_in_storage_real
    #define ddcl_new_storage ddcl_new_storage_real
#endif


typedef dduint32 ddcl_Handle;
struct tag_ddcl_Storage;
typedef struct tag_ddcl_Storage ddcl_Storage;

DDCLAPI int
ddcl_init_storage_module (ddcl * conf);

DDCLAPI void
ddcl_exit_storage_module ();

DDCLAPI void
ddcl_print_storage_info ();

DDCLAPI ddcl_Storage *
ddcl_new_storage_real (unsigned ele_sz, unsigned init_count);

DDCLAPI void
ddcl_free_storage (ddcl_Storage * s);

DDCLAPI ddcl_Handle
ddcl_register_in_storage_real (ddcl_Storage * s, void ** p);

DDCLAPI void *
ddcl_find_in_storage (ddcl_Storage * hs, ddcl_Handle h);

DDCLAPI int
ddcl_del_in_storage (ddcl_Storage * hs, ddcl_Handle h);

DDCLAPI void
ddcl_expand_storage (ddcl_Storage * hs, dduint32 size);

DDCLAPI void
ddcl_begin_storage(ddcl_Storage * hs);

DDCLAPI int
ddcl_next_storage(ddcl_Storage * hs, ddcl_Handle * h, void ** p);


#ifdef DDMALLOC_RECORD

DDCLAPI ddcl_Storage *
ddcl_new_storage_record (
        unsigned ele_sz, unsigned init_count, char * file, int line);

DDCLAPI ddcl_Handle
ddcl_register_in_storage_record (
        ddcl_Storage * s, void ** p, char * file, int line);

#endif
