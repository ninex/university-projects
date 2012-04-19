#include "include/crt.h"
#include "include/ipc.h"
#include "include/sys_calls.h"
int main() {
  int handle;
  int i=1,k;
  
  handle = crt_open();
  int port = 0;
  while (ipc_lookup_port(port) != IPC_OK)
  {
    port++;
  }
  crt_printf(handle,"Port %d found\n",port);
  ipc_send(port,&i,sizeof(int),&k,sizeof(int));
  while (1) {
    
    crt_printf(handle,"k = %d\n",k);
  }
  
  return 0;
}
