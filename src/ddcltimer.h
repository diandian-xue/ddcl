#pragma once

#include "ddclconfig.h"
#include "ddcl.h"
#include "ddclthread.h"
#include "ddclservice.h"

DDCLAPI int
ddcl_init_timer_module (ddcl * conf);

DDCLAPI void
ddcl_exit_timer_module ();

DDCLAPI dduint64
ddcl_now ();

DDCLAPI dduint64
ddcl_systime ();

DDCLAPI void
ddcl_add_timeout (ddcl_Handle h, ddcl_Session session, dduint32 ms);
