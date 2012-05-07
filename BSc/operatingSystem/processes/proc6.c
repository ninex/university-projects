#include "include/crt.h"
#include "include/sys_calls.h"
#include "include/ipc.h"

typedef struct buf_type {
	int i1;
	int i2;
} buf_type;

#define PORT1 3
#define PORT2 2

char * num_str(int i) {
	switch (i){
		case 0:
			return "Lighter";
		case 1:
			return "Tobacco";
		case 2:
			return "Paper";

	}
	return "???";
}

int a, m, q, r, z;

int uniform(int max) {
  int x, gamma;
  gamma = (a*(z % q))-(r*(z/q));
  if (gamma > 0)
    z = gamma;
  else
    z = gamma+m;
  x = z%m;
  while (x > max)
    x = x/10;
    
  return x % max;
}	


void seed()
{
	a = 16087;
	m = 2147483647;
	q = m / a;
	r = m % a;
	z = 1234; /* seed */
}

int main() {
	int handle;
	buf_type buf;
	int i,j;
	int served[3] = {0,0,0};


	handle = crt_open();
	seed();
	crt_printf(handle,"Creating port %d\n",PORT1);
	if (ipc_create_port(PORT1) != IPC_OK) {
		crt_printf(handle,"Error creating port %d!\n",PORT1);
  while(1);
		return 0;
	}
	crt_printf(handle,"Creating port %d\n",PORT2);
	if (ipc_create_port(PORT2) != IPC_OK) {
		crt_printf(handle,"Error creating port %d!\n",PORT2);
  while(1);
		return 0;
	}
	
	crt_printf(handle,"Ports Created\n");

	
	crt_printf(handle,"Assigning Client Numbers\n");
	i = 0;
	
	while (i < 3) 
	{
		if (ipc_receive(PORT2,&j,sizeof(int)) != IPC_OK) 
		{
			crt_printf(handle,"ipc_receive didnt return OK!\n");
   while(1);
			return 0;
		}
		crt_printf(handle,"Assigning Client %d\n",i);
		if (ipc_reply(PORT2,&i,sizeof(int)) != IPC_OK) {
			crt_printf(handle,"ipc_reply didnt return OK!\n");
   while(1);
			return 0;
		}
		crt_printf(handle,"Increasing \n");
		i++;
	}
	i = 1;
	while (1) {
		if (i) {
			crt_printf(handle,"Getting new items\n");
			buf.i1 = uniform(3);
			buf.i2 = buf.i1;
			while (buf.i1 == buf.i2)
				buf.i2 = uniform(3);
		}
		
		crt_printf(handle,"Agent ready, holding %s and %s\n",num_str(buf.i1),num_str(buf.i2));
		

		if (ipc_receive(PORT1,&i,sizeof(int)) != IPC_OK) {
			crt_printf(handle,"ipc_receive didnt return OK!\n");
   while(1);
			return 0;
		}
		
		crt_printf(handle,"Client %d who holds %s requested the items.\n",i,num_str(i));
		
		if (ipc_reply(PORT1,&buf,sizeof(struct buf_type)) != IPC_OK) {
			crt_printf(handle,"ipc_reply didnt return OK!\n");
   while(1);
			return 0;
		}
		crt_printf(handle,"Given items to the client. Waiting for client to finnish\n");
		
		if (ipc_receive(PORT2,&i,sizeof(int)) != IPC_OK) {
			crt_printf(handle,"ipc_receive didnt return OK!\n");
   while(1);
			return 0;
		}
		
		if (i) {
			crt_printf(handle,"Client used the items and is now done.\n");
			served[i] ++;
		} else {
			crt_printf(handle,"Client returned the items without using them.\n");
		}
		
		if (ipc_reply(PORT2,&i,sizeof(int)) != IPC_OK) {
			crt_printf(handle,"ipc_reply didnt return OK!\n");
   while(1);
			return 0;
		}
		
		
	}
	crt_printf(handle,"Closing port %d\n",PORT1);
	ipc_close_port(PORT1);
	crt_printf(handle,"Closing port %d\n",PORT2);
	ipc_close_port(PORT2);
	crt_printf(handle,"Ports closed\nTerminating\n");
while(1);
	return 0;
}
