#ifndef COMMS_H
#define COMMS_H

#define COMMS_MAX_PORTS 32
#define COMMS_MAX_MSG 0x8000

typedef struct msg
{
  void *req_buf,*rep_buf;
  unsigned int req_size,rep_size,offset;
  int mult;
  struct msg *next;
  page_directory* pd;  
}msg;

typedef struct str_msg_port
{
  struct msg *queue, *last;
  process_control_block* pcb;
  unsigned int index;
  process_list *bq,*bq_last,*server;
}str_msg_port;

int comms_lookup_port(unsigned int);

int comms_create_port(unsigned int);

int comms_send(cpu_state *state);

int comms_receive(cpu_state *state);

int comms_reply(unsigned int port, void *buf, unsigned int size);

int comms_close_port(unsigned int port);

#endif
