
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ddcldson.h"

#include "luaddcl.h"
#include "luaddcl_service.h"
#include "lua_cjson.h"


static const char SCRIPT[] = ""\
"ddcl.start(function() \n" \
    "ddcl.callback(function(...) \n" \
        "local msg, script, sz = ... \n" \
        "if script == \"\" or script == \"\\n\" then \n" \
            "return \n" \
        "end \n" \
        "local f, err = load(script) \n" \
        "assert(f, err) \n" \
        "ddcl.log(f()) \n" \
        "if msg[3] > 0 then ddcl.resp([[success]]) end \n"\
    "end) \n" \
"end) \n" \
"";

#define LUAPATH "./?.lua"
#ifdef DD_WINDOWS
#define LUACPATH "./?.dll"
#else
#define LUACPATH "./?.so"
#endif


static int LUAPATH_ARGV_I = 1;
static char * LUAPATH_ARGV[32] = {NULL};
static int LUACPATH_ARGV_I = 1;
static char * LUACPATH_ARGV[32] = {NULL};

static char * SCRIPT_FILE = NULL;
static char * SCRIPT_STAT = NULL;
static int IS_DEAMON = 0;

#define CHECK_ARGV(I, ARGC) {if((++I) >= ARGC){ return 1; }}

static int
parse_args (int argc, char ** argv){
    int i = 1;
    while(i < argc){
        if(argv[i][0] != '-'){
            SCRIPT_FILE = argv[i];
            i++;
            continue;
        }
        switch(argv[i][1]){
        case 'e':
            CHECK_ARGV(i, argc);
            SCRIPT_STAT = argv[i];
            break;
        case 'l':
            CHECK_ARGV(i, argc);
            LUAPATH_ARGV[LUAPATH_ARGV_I++] = argv[i];
            break;
        case 'L':
            CHECK_ARGV(i, argc);
            LUACPATH_ARGV[LUACPATH_ARGV_I++] = argv[i];
            break;
        case 'd':
            IS_DEAMON = 1;
            break;
        default:
            return 1;
            break;

        }
        i ++;
    }

    return 0;
}


static int
open_dd_library(lua_State * L){
  luaopen_libluaddcl_core(L);
  lua_setglobal(L, "ddcl");

  luaopen_cjson(L);
  lua_setglobal(L, "cjson");
  return 0;
}


int main (int argc, char **argv) {
    memset(LUAPATH_ARGV, 0, sizeof(LUAPATH_ARGV));
    memset(LUACPATH_ARGV, 0, sizeof(LUACPATH_ARGV));
    LUAPATH_ARGV[0] = LUAPATH;
    LUACPATH_ARGV[0] = LUACPATH;

    int ret = parse_args(argc, argv);
    if(ret){
        fprintf(stderr, "args error %d\n", ret);
        return ret;
    }

    ddcl conf;
    ddcl_default(&conf);
    ddcl_init(&conf);

    lddcl_set_newservice_hook(open_dd_library);

    ddcl_Service svr = lddcl_new_luaservice(
            NULL, LUAPATH_ARGV, LUACPATH_ARGV, SCRIPT, NULL, 0, 0);

    ddcl_DsonBuffer * dson = ddcl_new_dsonbuffer(0);
    size_t dsonlen;

    if(SCRIPT_STAT){
        ddcl_clear_dsonbuffer(dson);
        dson = ddcl_push_dsonbuffer_string(dson, SCRIPT_STAT, strlen(SCRIPT_STAT));
        dsonlen = ddcl_dsonbuffer_len(dson);
        ddcl_send_b(svr, 0, DDCL_PTYPE_DSON | DDCL_PTYPE_SEND,
                DDCL_CMD_TEXT, 0, (void *) dson, dsonlen);
    }else if(SCRIPT_FILE){
        char dofile_scipt[512];
        sprintf(dofile_scipt, "return dofile([[%s]])", SCRIPT_FILE);
        ddcl_clear_dsonbuffer(dson);
        dson = ddcl_push_dsonbuffer_string(dson, dofile_scipt, strlen(dofile_scipt));
        dsonlen = ddcl_dsonbuffer_len(dson);
        ddcl_send_b(svr, 0, DDCL_PTYPE_DSON | DDCL_PTYPE_SEND,
                DDCL_CMD_TEXT, 0, (void *)dson, dsonlen);
    }

    if(IS_DEAMON){
        for(;;){
            ddcl_sleepms(1000000);
        }
        return 0;
    }

    char buffer[512];
    char * read = 0;
    for(;;){
        read = fgets(buffer, 512, stdin);
        if(read == NULL){
            break;
        }
		if (strcmp(read, "exit\n") == 0) {
			break;
		}
        ddcl_clear_dsonbuffer(dson);
        dson = ddcl_push_dsonbuffer_string(dson, read, strlen(read));
        dsonlen = ddcl_dsonbuffer_len(dson);
        ddcl_send_b(svr, 0, DDCL_PTYPE_DSON | DDCL_PTYPE_SEND,
                DDCL_CMD_TEXT, 0, (void *)dson, dsonlen);
    }

    ddcl_final();
    return 0;

}

