#include "include/crt.h"
#include "include/ipc.h"
#include "include/sys_calls.h"
int main() {
  int handle;
  unsigned int i=1,k;
  
  handle = crt_open();
  int port = 0;
  while (ipc_lookup_port(port) != IPC_OK)
  {
    port++;
    if (port == 32)
      port = 0;
  }
  crt_printf(handle,"Port %d found\n Ready to send number to increment to server",port);
  ipc_send(port,&i,sizeof(unsigned int),&k,sizeof(unsigned int));
  //crt_printf(handle,"was i blocked?\n");
//   ipc_send(port,&i,sizeof(unsigned int),&k,sizeof(unsigned int));
//   crt_printf(handle,"k = %d\n",k);
  while (1) {
    i=k;
    if (ipc_lookup_port(port) == IPC_OK)
    {
      crt_gotoxy(handle,0,3);
      crt_printf(handle,"k = %d",k);
      ipc_send(port,&i,sizeof(unsigned int),&k,sizeof(unsigned int));      
      //crt_printf(handle,"was i blocked? k = %d\n",k);
    }else
    {
      //crt_printf(handle,"DONE\n");
    }
  }
  
  return 0;
}
