﻿#define DDCL_CORE

#include "ddclmalloc.h"
#include "ddclmap.h"
#include "ddclthread.h"

#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

static ddcl_SpinLock _lock;
static ddcl_Map * _map;


DDCLAPI int
ddcl_init_malloc_module (){
    _map = ddcl_new_map(NULL, NULL);
	//ddcl_expand_map(_map, 128);
    ddcl_init_spin(&_lock);
    return 0;
}

DDCLAPI void
ddcl_exit_malloc_module (){
    ddcl_destroy_spin(&_lock);
    ddcl_free_map(_map);
}

DDCLAPI void *
ddcl_mallocR(size_t sz, char * r, int line){
    void * p = malloc(sz);
    char s[256];
    snprintf(s, 256, "%s:%d", r, line);
    ddcl_lock_spin(&_lock);
    ddcl_set_map(_map, &p, sizeof(void*), s, 256);
    ddcl_unlock_spin(&_lock);
    return p;
}

DDCLAPI void
ddcl_freeR(void * p){
    free(p);
    ddcl_lock_spin(&_lock);
    char * s = (char*)ddcl_set_map(_map, &p, sizeof(void *), NULL, 0);
    ddcl_unlock_spin(&_lock);
}

DDCLAPI void *
ddcl_realloc(void * p, size_t sz){
    void * np = realloc(p, sz);
    return np;
}

DDCLAPI void
ddcl_print_malloc_info (){
    ddcl_lock_spin(&_lock);
    ddcl_begin_map(_map);
    char * s;
    void * p;
    while(ddcl_next_map(_map, &p, NULL, (void **)&s, NULL)){
        printf("un free:%p %s\n", (void *)(void **)p, s);
    }
    fflush(stdout);
    ddcl_unlock_spin(&_lock);
}