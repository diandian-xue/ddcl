#define DDCL_CORE

#include "ddcl.h"

#include "ddclfile.h"
#include "ddclmalloc.h"
#include "ddclservice.h"
#include "ddcltimer.h"
#include "ddclsocket.h"
#include "ddcllog.h"
#include "ddclmalloc.h"

#define CKRETURN(X) { int r = X(conf); if(r != 0) return r; }

DDCLAPI int
ddcl_init (ddcl * conf){
    CKRETURN(ddcl_init_malloc_module);
    CKRETURN(ddcl_init_storage_module);
    CKRETURN(ddcl_file_module_init);
    CKRETURN(ddcl_init_service_module);
    CKRETURN(ddcl_init_log_module);
    CKRETURN(ddcl_init_timer_module);
    CKRETURN(ddcl_init_socket_module);
    return 0;
}

DDCLAPI void
ddcl_final (){
    ddcl_exit_socket_module();
    ddcl_exit_service_module();
    ddcl_exit_timer_module();

    ddcl_print_storage_info();
    ddcl_exit_storage_module();
    ddcl_print_malloc_info();
    ddcl_exit_malloc_module();
}

DDCLAPI void
ddcl_default (ddcl * conf){
    conf->worker = 1;
    conf->socket = 1;
    conf->timer_ms = 1;
}
