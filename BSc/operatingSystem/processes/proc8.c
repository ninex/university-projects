#include "include/crt.h"
#include "include/ipc.h"
#include "clntsvr.h"

static message request;
static message reply;

int main() {
  int handle;
  int i;
  int res;
  unsigned char ch;
  
  handle = crt_open();
  i = 0;
  res = ipc_lookup_port(PORT);
  while (res != IPC_OK) {
    crt_gotoxy(handle, 0, 0);
    crt_printf(handle, "[client] locating port=%d (%d)\n", i, res);
    res = ipc_lookup_port(PORT);
    i++; 
  }
  crt_printf(handle, "[client] port located\n");
  
  while(1) {
    crt_printf(handle, "[client] press any key to send message\n");
    do {
    } while (crt_key_available(handle) == 0);
    ch = crt_read_key(handle);
    request.x = 5;
    request.y = 7;
    request.z = 0;
    reply.x = 0;
    reply.y = 0;
    reply.z = 0;
    res = ipc_send(PORT, &request, sizeof(message), &reply, sizeof(message));
    if (res == IPC_OK)
      crt_printf(handle, "[client] reply received (x=%d, y=%d, z=%d)\n", 
                 reply.x, reply.y, reply.z);
    else
      crt_printf(handle, "[client] error sending message (%d)\n", res);
  }
  
  return 0;
}
