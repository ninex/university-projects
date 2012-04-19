#ifndef COMMS_H
#define COMMS_H

#define COMMS_MAX_PORTS 32
#define COMMS_MAX_MSG 0x8000

typedef struct msg
{
  void *req_buf,*rep_buf;
  unsigned int req_size,rep_size;
  struct msg *next;
  page_directory* pd;  
}msg;

typedef struct str_msg_port
{
  struct msg *queue;
  struct msg *last;
  page_directory* pd;
}str_msg_port;

int comms_lookup_port(unsigned int);

int comms_create_port(unsigned int);

int comms_send(unsigned int port, void *req_buf, unsigned int req_size, void *rep_buf, unsigned int rep_size);

int comms_receive(unsigned int port, void *buf, unsigned int size);

int comms_reply(unsigned int port, void *buf, unsigned int size);

int comms_close_port(unsigned int port);

#endif