#pragma once

enum {
    DDCL_PTYPE_SEND = 1,
    DDCL_PTYPE_RESP = 1 << 1,
    DDCL_PTYPE_DSON = 1 << 2, //dson 格式数据标识
    DDCL_PTYPE_KEEP_SS = 1 << 3, //保留session标识

    DDCL_PTYPE_END,
};

enum {
    DDCL_CMD_TEXT = 1,
    DDCL_CMD_EXIT,
    DDCL_CMD_TIMEOUT,
    DDCL_CMD_ERROR,
    DDCL_CMD_SOCKET,
    DDCL_CMD_LOG,
    DDCL_CMD_START,
    DDCL_CMD_DEL_SESSION,

    DDCL_CMD_END,
};


typedef dduint64 ddcl_Session;
typedef ddcl_Handle ddcl_Service;

typedef struct tag_ddcl_Msg{
    int ptype;
    int cmd;
    void * ud;
    ddcl_Service from;
    ddcl_Service self;
    ddcl_Session session;
    void * data;
    size_t sz;
    int free;
}ddcl_Msg;
typedef void (*ddcl_MsgCB)(ddcl_Msg *);
