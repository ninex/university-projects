#include "include/ipc.h"
#include "include/sys_calls.h"

int ipc_lookup_port(unsigned int port) {
  /*return invalid port when port exceeds max ports
    return no such port when the port doesn't exist yet
    return OK when requested port could be located*/
  return (_sys_1(SYS_COMMS_LOOKUP_PORT,port));
}

int ipc_create_port(unsigned int port) {
  /*return duplicate port when the port exists
    return invalid port if port exceeds max ports
    return OK*/
  return (_sys_1(SYS_COMMS_CREATE_PORT,port));
}

int ipc_send(unsigned int port, void *req_buf, unsigned int req_size, void *rep_buf, unsigned int rep_size) {
  /*return no such port when if the port doesn't exist
    return invalid port if port exceeds max ports
    return invalid msg size if req_size or rep_size is less than 1 or exceeds max msg size
    return invalid buffer when a buffer equals NULL
    return OK*/
    return (_sys_5(SYS_COMMS_SEND,port,req_buf,req_size,rep_buf,rep_size));
    
}

int ipc_receive(unsigned int port, void *buf, unsigned int size) {
  /*return no such port when if the port doesn't exist
    return invalid port if port exceeds max ports
    return invalid msg size if req_size or rep_size is less than 1 or exceeds max msg size
    return invalid buffer when a buffer equals NULL
    return OK*/
  return (_sys_3(SYS_COMMS_RECEIVE,port,buf,size));
}

int ipc_reply(unsigned int port, void *buf, unsigned int size) {
  /*return no such port when port doesn't exist
    return invalid port when port exceeds max ports
    return invalid msg size when msg is smaller than 1 or bigger than msg max
    return invalid buffer when a buffer equals NULL
    return OK
  */
  return (_sys_3(SYS_COMMS_REPLY,port,buf,size));
}

int ipc_close_port(unsigned int port) {
  /*return error no such port if port doesn't exist
    return invalid port if port < 0 or bigger than max ports
    return OK*/
    return (_sys_1(SYS_COMMS_CLOSE_PORT,port));
}
