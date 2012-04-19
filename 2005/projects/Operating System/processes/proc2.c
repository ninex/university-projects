#include "include/crt.h"
#include "include/ipc.h"

int main() {
  int handle;
  unsigned int i,port=1;
  unsigned char c = '0',d;
  
  handle = crt_open();

  
  while (1) {
    d = crt_read_key(handle);
    switch (d)
    {
      case 'h':
        crt_gotoxy(handle, 0, 0);
        crt_clear_display(handle);
        crt_printf(handle, "Test comms\n'l' = lookup port\n'c' = create port\n's' = send\n'r' = receive\n'p' = reply\n'k' = close port\n'1' = choose port 1\n'2' = choose port 2\n");
        break;
      case 'l':
        i = ipc_lookup_port(port);
        crt_printf(handle,"[Port %d]lookup status returned = %d\n",port,i);
        break;
      case 'c':
        i = ipc_create_port(port);
        crt_printf(handle,"[Port %d]create status returned = %d\n",port,i);
        break;
      case 's':
        i = ipc_send(port,0,0,0,0);
        crt_printf(handle,"[Port %d]send status returned = %d\n",port,i);
        break;
      case 'r':
        i = ipc_receive(port,0,0);
        crt_printf(handle,"[Port %d]receieve status returned = %d\n",port,i);
        break;
      case 'p':
        i = ipc_reply(port,0,0);
        crt_printf(handle,"[Port %d]reply status returned = %d\n",port,i);
        break;
      case 'k':
        i = ipc_close_port(port);
        crt_printf(handle,"[Port %d]close status returned = %d\n",port,i);
        break; 
      case '1':
        port = 1;
        crt_printf(handle,"[Port 1]\n");
        break;
      case '2':
        port = 2;
        crt_printf(handle,"[Port 2]\n");
        break;           
    }
  }
  
  return 0;
}
