#include "include/crt.h"
#include "include/ipc.h"
#include "clntsvr.h"

static message request;
static message reply;

int main() {
  int handle;
  int res;
  unsigned char ch;
  
  handle = crt_open();
  
  crt_gotoxy(handle, 0, 0);
  crt_printf(handle, "[server] press any key to create port\n");
  
  do {
  } while (crt_key_available(handle) == 0);
  ch = crt_read_key(handle);  

  res = ipc_create_port(PORT);

  if (res == IPC_OK)
    crt_printf(handle, "[server] port successfully created\n");
  else
    crt_printf(handle, "[server] error creating port (%d)\n", res);
  
  while(1) {
    crt_printf(handle, "[server] press any key to receive message\n");
    do {
    } while (crt_key_available(handle) == 0);
    ch = crt_read_key(handle);
    request.x = 0;
    request.y = 0;
    request.z = 0;
    reply.x = 0;
    reply.y = 0;
    reply.z = 0;
    res = ipc_receive(PORT, &request, sizeof(message));
    if (res == IPC_OK) {
      crt_printf(handle, "[server] request received (x=%d, y=%d, z=%d)\n", 
                 request.x, request.y, request.z);
      reply.x = 1;
      reply.y = 1;
      reply.z = request.x+request.y;
      crt_printf(handle, "[server] press any key to send reply\n");
      do {
      } while (crt_key_available(handle) == 0);
      ch = crt_read_key(handle);
      res = ipc_reply(PORT, &reply, sizeof(message));
      if (res == IPC_OK) 
        crt_printf(handle, "[server] reply successfully send\n");
      else
        crt_printf(handle, "[server] error sending reply (%d)\n", res);
    } else {
      crt_printf(handle, "[server] error receiving message (%d)\n", res);
    }
  }  
  return 0;
}
