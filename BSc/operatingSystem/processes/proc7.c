#include "include/crt.h"
#include "include/sys_calls.h"
#include "include/ipc.h"
#include "include/asm.h"
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

//#define CLIENTNO 1
int main() {
	int handle;
	int i, i1,i2, served = 0, smoke;
	buf_type buf;
	
	handle = crt_open();

	crt_printf(handle,"Checking if ports %d and %d exist\n",PORT1,PORT2);

	while (ipc_lookup_port(PORT1) != IPC_OK);
	while (ipc_lookup_port(PORT2) != IPC_OK);
	crt_printf(handle,"Found ports!\n");
	
	crt_printf(handle,"Getting Client Number\n");
	if (ipc_send(PORT2,&i,sizeof(int),&i,sizeof(int)) != IPC_OK) 
	{
		crt_printf(handle,"ipc_send didnt return OK!\n");
		while(1);
                return 0;
	}
	
	switch(i) {
		case 0: i1=0;i2=1;break;
		case 1: i1=1;i2=2;break;
		case 2: i1=2;i2=0;break;
	}

	crt_printf(handle,"I am client %d with items %s and %s\n",i,num_str(i1),num_str(i2));
	
	while (1) {

		if (ipc_send(PORT1,&i,sizeof(int),&buf,sizeof(struct buf_type)) != IPC_OK) {
			crt_printf(handle,"ipc_send didnt return OK!\n");
                         while(1);
			return 0;
		}
		
		crt_printf(handle,"The agent gave me %s and %s\n",num_str(buf.i1),num_str(buf.i2));
		smoke = 0;
		if (buf.i1 == i1) {
			if (buf.i2 == i2) {
				smoke = 1;
			}
		} else if (buf.i1 == i2) {
			if (buf.i2 == i1) {
				smoke = 1;
			}
		}
		if (smoke) {
			crt_printf(handle,"I have what I need, am now SMOKING!\nInforming agent I am done\n");
   //delay(100);
			served++;
		} else {
			crt_printf(handle,"I dont have what I need.\nReturning items to agent\n");
		}
		if (ipc_send(PORT2,&smoke,sizeof(int),&smoke,sizeof(int)) != IPC_OK) {
			crt_printf(handle,"ipc_send didnt return OK!\n");
   while(1);
			return 0;
		}
		
	}
 while(1);
	return 0;
}
