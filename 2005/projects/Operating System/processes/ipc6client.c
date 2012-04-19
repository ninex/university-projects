#include "include/crt.h"
#include "include/sys_calls.h"
#include "include/ipc.h"

#include "ipc6inc.h"
char x[M_SIZE];


int main() {
	int err;
	int a;

	handle = crt_open();

	
	crt_printf(handle,"Checking if port %d exists\n",P);

	while (ipc_lookup_port(P) != IPC_OK);
	crt_printf(handle,"Found port!\n");
	fill(x,f1);
	while (1) {
		crt_printf(handle,"Sending message\n");
		if ((err = ipc_send(P,x, M_SIZE, &a, sizeof(int))) != IPC_OK) {
			crt_printf(handle, "ipc_send didnt return OK! (%d)\n",err);
			return 0;
		}
		crt_printf(handle,"Got answer\n");
	}
	return 0;
}

