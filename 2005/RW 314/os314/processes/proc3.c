#include "include/crt.h"
#include "include/sys_calls.h"
#include "include/ipc.h"

int main() {
  int handle;
  int i=0;
  
  handle = crt_open();

  int port=0;
  while (ipc_create_port(port) != IPC_OK)
  {
    port++;
  }
  crt_printf(handle,"Port %d created  \n",port);
  while (1)
  {
    if (ipc_receive(port,&i,sizeof(int)) == IPC_OK)
    {
     crt_printf(handle, "Message recieved\n");
     crt_printf(handle,"i=%d\n",i);
     i++;
     crt_printf(handle,"i = %d\n",i);
     ipc_reply(port,&i,sizeof(int));
    }else
    {
      
    }
  }  
  sys_0(SYS_EXIT);
  //return 0;
}
