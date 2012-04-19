#include "include/crt.h"
#include "include/sys_calls.h"
#include "include/ipc.h"

#include "ipc6inc.h"

char x[M_SIZE];


int main() {
	int a;
	handle = crt_open();

	crt_printf(handle,"Creating port %d\n",P);
	if (ipc_create_port(P) != IPC_OK) {
		crt_printf(handle,"Error creating port!\n");
		return 0;
	}
	crt_printf(handle,"Port Created\n");

	while (1) {
		crt_printf(handle,"Starting to Receive\n");

		if (ipc_receive(P,x,M_SIZE) != IPC_OK) {
			crt_printf(handle,"ipc_receive didnt return OK!\n");
			return 0;
		}
		crt_printf(handle,"Got send, Verifying x[] with f1\n");
		if (!verify(x,f1)) {
			crt_printf(handle,"ERROR!! Didnt get correct answer!\n");
			return 0;
		}
		crt_printf(handle,"Verify good.\nReplying\n");
		if (ipc_reply(P,&a,sizeof(int)) != IPC_OK) {
			crt_printf(handle,"ipc_reply didnt return OK!\n");
			return 0;
		}
		crt_printf(handle,"Done reply\n\n");
	}
	crt_printf(handle,"Closing port %d\n",P);
	ipc_close_port(P);
	crt_printf(handle,"Port closed\nTerminating\n");

	return 0;
}

