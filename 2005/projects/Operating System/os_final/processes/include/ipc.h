#ifndef IPC_H
#define IPC_H

#define IPC_OK 0

#define ERR_IPC_DUPLICATE_PORT       600
#define ERR_IPC_NO_SUCH_PORT         601
#define ERR_IPC_INVALID_MSG_SIZE     602
#define ERR_IPC_INVALID_PORT         603
#define ERR_IPC_INVALID_BUFFER       604
#define ERR_IPC_REPLY_MISMATCH       605
#define ERR_IPC_CLOSE_PORT           606

int ipc_lookup_port(unsigned int);

int ipc_create_port(unsigned int);

int ipc_send(unsigned int port, void *req_buf, unsigned int req_size, void *rep_buf, unsigned int rep_size);

int ipc_receive(unsigned int port, void *buf, unsigned int size);

int ipc_reply(unsigned int port, void *buf, unsigned int size);

int ipc_close_port(unsigned int port);

#endif
