#include "include/crt.h"
#include "include/sys_calls.h"
#include "include/ipc.h"

int main() {
  int handle;
  unsigned int i=0,result;
  
  handle = crt_open();

  int port=1;
  while (ipc_create_port(port) != IPC_OK)
  {
    port++;
  }
  crt_printf(handle,"Port %d created\nReceiving number to increment",port);
  while (1)
  {
    result=ipc_receive(port,&i,sizeof(unsigned int));
    //if (result == IPC_OK)
    //  {
       //crt_printf(handle, "Message recieved\n");
       //crt_printf(handle,"i=%d\n",i);
       i++;
       //crt_printf(handle,"i = %d\n",i);
       ipc_reply(port,&i,sizeof(unsigned int));
    //  }else
    //  {
        //crt_printf(handle,"hoekom? result = %d\n",result);
    //  }    
  }  
  sys_0(SYS_EXIT);
  //return 0;
}
