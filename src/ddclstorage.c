#define DDCL_CORE

#include "ddcl.h"
#include "ddclstorage.h"
#include "ddclmalloc.h"
#include "ddclthread.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct tag_Block{
    char used;
    ddcl_Handle h;
    struct tag_Block * next;
    struct tag_Block * last;
}Block;

typedef struct tag_Slot {
    char * buf;
    dduint32 size;
    struct tag_Slot * next;
}Slot;

struct tag_ddcl_Storage{
    ddcl_Handle h_index;
    dduint32 size;
    dduint32 ele_sz;
    Slot * slot;
    Slot * slot_end;

    Block * block;
    Block * block_end;
    Block * block_cur;
};

#ifdef DDMALLOC_RECORD
struct {
    ddcl_SpinLock lock;
    ddcl_Map * map;
}_R;
#endif

#define MAX_EXPEND_SIZE     10000

static Slot *
_new_slot(dduint32 ele_sz, dduint32 size) {
    if(size > MAX_EXPEND_SIZE){
        size = MAX_EXPEND_SIZE;
    }
    Slot * s = ddcl_malloc(sizeof(Slot));
    s->buf = ddcl_malloc((sizeof(Block) + ele_sz) * size);
    memset(s->buf, 0, (sizeof(Block) + ele_sz) * size);
    s->size = size;
    s->next = NULL;
    return s;
}

static void
_free_slot(Slot * s) {
    Slot * tmp;
    while (s) {
        tmp = s;
        s = s->next;
        ddcl_free(tmp->buf);
        ddcl_free(tmp);
    }
}

static char *
_register_in_slot(ddcl_Storage * hs, ddcl_Handle h) {
    Block * block;
    ddcl_Handle hash;
    Slot * s = hs->slot;
    while (s) {
        hash = h % s->size;
        block = (Block *)&(s->buf[hash * (sizeof(Block) + hs->ele_sz)]);
        if (!block->used) {
            block->used = 1;
            block->h = h;
            if(!hs->block){
                hs->block = block;
                hs->block_end = block;
            }else{
                hs->block_end->next = block;
                block->last = hs->block_end;
                hs->block_end = block;
            }
            return ((char *)block) + sizeof(Block);
        }
        s = s->next;
    }
    return NULL;
}

static Block *
_find_in_slot(ddcl_Storage * hs, ddcl_Handle h) {
    Block * block;
    ddcl_Handle hash;
    Slot * s = hs->slot;
    while (s) {
        hash = h % s->size;
        block = (Block *)&(s->buf[hash * (sizeof(Block) + hs->ele_sz)]);
        if(block->used && block->h == h){
            return block;
        }
        s = s->next;
    }
    return NULL;
}

DDCLAPI int
ddcl_init_storage_module (ddcl * conf){
#ifdef DDMALLOC_RECORD
    _R.map = ddcl_new_map(NULL, NULL);
    ddcl_expand_map(_R.map, 128);
    ddcl_init_spin(&(_R.lock));
#endif
    return 0;
}

DDCLAPI void
ddcl_exit_storage_module (){
#ifdef DDMALLOC_RECORD
    ddcl_free_map(_R.map);
    ddcl_destroy_spin(&(_R.lock));
#endif
}

DDCLAPI void
ddcl_print_storage_info (){
#ifdef DDMALLOC_RECORD
    ddcl_lock_spin(&(_R.lock));
    ddcl_begin_map(_R.map);
    char * s;
    void * p;
    while(ddcl_next_map(_R.map, &p, NULL, (void **)&s, NULL)){
        printf("malloc leaked:%p %s\n", (void *)(void **)p, s);
    }
    fflush(stdout);
    ddcl_unlock_spin(&(_R.lock));
#endif
}

DDCLAPI ddcl_Storage *
ddcl_new_storage_real (unsigned ele_sz, unsigned initSize){
    if (initSize < 1)
        initSize = 1;
    ddcl_Storage * hs = ddcl_malloc(sizeof(ddcl_Storage));
    memset(hs, 0, sizeof(ddcl_Storage));
    hs->h_index = 0;
    hs->size = initSize;
    hs->ele_sz = ele_sz;
    hs->slot = _new_slot(ele_sz, initSize);
    hs->slot_end = hs->slot;
    return hs;
}

DDCLAPI void
ddcl_free_storage (ddcl_Storage * hs){
    _free_slot(hs->slot);
    ddcl_free(hs);

#ifdef DDMALLOC_RECORD
    ddcl_lock_spin(&(_R.lock));
    ddcl_set_map(_R.map, &hs, sizeof(void *), NULL, 0);
    ddcl_unlock_spin(&(_R.lock));
#endif
}

DDCLAPI ddcl_Handle
ddcl_register_in_storage_real (ddcl_Storage * hs, void ** p){
    ddcl_Handle handle;
    char * buf;
    dduint32 h_index = hs->h_index;
    for(;;){
        for(dduint32 i = 0; i < hs->size; i ++){
            handle = ++h_index;
            if (handle == 0)
                handle = ++h_index;
            buf = _register_in_slot(hs, handle);
            if(buf){
                *p = buf;
                hs->h_index = h_index;
                return handle;
            }
        }
        ddcl_expand_storage(hs, hs->slot_end->size * 2);
    }
}

DDCLAPI void *
ddcl_find_in_storage (ddcl_Storage * hs, ddcl_Handle h){
    Block * block = _find_in_slot(hs, h);
    if (block) {
        return ((char *)block) + sizeof(Block);
    }
    return NULL;
}

DDCLAPI int
ddcl_del_in_storage (ddcl_Storage * hs, ddcl_Handle h){
    Block * block = _find_in_slot(hs, h);
    if (block) {
        if(hs->block == block){
            hs->block = block->next;
        }
        if(hs->block_end == block){
            hs->block_end = block->last;
        }
        if(hs->block_cur == block){
            hs->block_cur = block->next;
        }
        if(block->last){
            block->last->next = block->next;
        }
        if(block->next){
            block->next->last = block->last;
        }
        memset(block, 0, sizeof(Block)+ hs->ele_sz);
        return 0;
    }
    return 1;
}

DDCLAPI void
ddcl_expand_storage (ddcl_Storage * hs, dduint32 size){
    Slot * s = _new_slot(hs->ele_sz, size);
    hs->slot_end->next = s;
    hs->slot_end = s;
    hs->size += size;
}

DDCLAPI void
ddcl_begin_storage(ddcl_Storage * hs){
    hs->block_cur = hs->block;
}

DDCLAPI int
ddcl_next_storage(ddcl_Storage * hs, ddcl_Handle * h, void ** p){
    Block * cur = hs->block_cur;
    if(!cur){
        return 0;
    }
    *h = cur->h;
    *p = ((char *)cur) + sizeof(Block);
    hs->block_cur = cur->next;
    return 1;
}

#ifdef DDMALLOC_RECORD

DDCLAPI ddcl_Storage *
ddcl_new_storage_record (
        unsigned ele_sz, unsigned init_count, char * file, int line){
    ddcl_Storage * s = ddcl_new_storage_real(ele_sz, init_count);
    char info[256];
    snprintf(info, 256, "%s:%d", file, line);
    ddcl_lock_spin(&(_R.lock));
    ddcl_set_map(_R.map, &s, sizeof(ddcl_Storage *), info, 256);
    ddcl_unlock_spin(&(_R.lock));
    return s;
}

DDCLAPI ddcl_Handle
ddcl_register_in_storage_record (
        ddcl_Storage * s, void ** p, char * file, int line){
    return ddcl_register_in_storage_real(s, p);
}

#endif
