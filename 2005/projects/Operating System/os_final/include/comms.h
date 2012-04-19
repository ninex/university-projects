#ifndef COMMS_H
#define COMMS_H

#define COMMS_MAX_PORTS 32
#define COMMS_MAX_MSG 0x8000

//struct for a msg
typedef struct msg
{
  void *req_buf,*rep_buf;
  unsigned int req_size,rep_size,offset;
  int mult;
  struct msg *next;
  page_directory* pd;  
}msg;

//struct for a port
typedef struct str_msg_port
{
  struct msg *queue, *last;
  process_control_block* pcb;
  unsigned int index,count;
  process_list *bq,*bq_last,*server;
}str_msg_port;

void update_all_port_error(unsigned int pd,int error);

//check if a port is available
int comms_lookup_port(unsigned int);

//create a port
int comms_create_port(unsigned int);

//send a msg to a port
int comms_send(cpu_state *state);

//recieve a msg on a port
int comms_receive(cpu_state *state);

//reply on the msg on a port
int comms_reply(unsigned int port, void *buf, unsigned int size);

//close a port
int comms_close_port(unsigned int port);

#endif
