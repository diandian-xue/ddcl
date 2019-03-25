#include <stdio.h>
#include <stdlib.h>

#include "ddcldson.h"

#include "lua_ddcl.h"
#include "lua_ddclservice.h"


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
        "if msg[3] > 0 then ddcl.resp([[success]], [[hello]]) end \n"\
    "end) \n" \
"end) \n" \
"";



static int open_dd_library(lua_State * L){
  luaopen_ddcllua_core(L);
  lua_setglobal(L, "ddcl");
  return 0;
}

int main (int argc, char **argv) {
    ddcl conf;
    ddcl_default(&conf);
    ddcl_init(&conf);

    lddcl_set_newservice_hook(open_dd_library);

    ddcl_Service svr = lddcl_new_luaservice(NULL, "", "", SCRIPT, "");

    ddcl_DsonBuffer * dson = ddcl_new_dsonbuffer(0);
    char * dsonbuf;
    size_t dsonlen;
    char buffer[512];
    char * read = 0;
    for(;;){
        read = fgets(buffer, 512, stdin);
        if(read == NULL){
            break;
        }
        ddcl_clear_dsonbuffer(dson);
        ddcl_push_dsonbuffer_string(dson, read, strlen(read));
        dsonbuf = ddcl_dsonbuffer_buffer(dson, &dsonlen);
        ddcl_send_b(svr, 0, DDCL_PTYPE_DSON | DDCL_PTYPE_SEND,
                DDCL_CMD_TEXT, 0, dsonbuf, dsonlen);
    }

    ddcl_final();
    return 0;

}
