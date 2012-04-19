#include "include/asm.h"
#include "include/error.h"
#include "include/stddef.h"
#include "include/mem.h"
#include "include/process.h"
#include "include/i386.h"
#include "include/io.h"
#include "include/comms.h"

static unsigned long comms_bitmap = 0xffffffff;
static str_msg_port *msg_port[COMMS_MAX_PORTS];

int comms_lookup_port(unsigned int port) {
  //return invalid port when port exceeds max ports
  if (port >= COMMS_MAX_PORTS)
  {
    return -ERR_IPC_INVALID_PORT;
  }
  //return no such port when the port doesn't exist yet
  if (bt(comms_bitmap,port))
  {
    return -ERR_IPC_NO_SUCH_PORT;
  }
  
  //return OK when requested port could be located*/  
  return OK;
}

int comms_create_port(unsigned int port) {
  //return duplicate port when the port exists
  if (bt(comms_bitmap,port) == 0)
  {
    return -ERR_IPC_DUPLICATE_PORT;
  }
  //return invalid port if port exceeds max ports
  if (port >= COMMS_MAX_PORTS)
  {
    return -ERR_IPC_INVALID_PORT;
  }
  
  
  //================================ create port================================================
  msg_port[port] = (struct str_msg_port*)mem_alloc(sizeof(struct str_msg_port));
  msg_port[port]->queue = NULL;
  msg_port[port]->last = NULL;
  msg_port[port]->pd = get_current()->pcb->pd;
  //set port as allocated
  btr(comms_bitmap,port);
  
  //return OK
  return OK;
}

int comms_send(unsigned int port, void *req_buf, unsigned int req_size, void *rep_buf, unsigned int rep_size) {
  //return no such port when if the port doesn't exist
  if (bt(comms_bitmap,port))
  {
    return -ERR_IPC_NO_SUCH_PORT;
  }
  //return invalid port if port exceeds max ports
  if (port >= COMMS_MAX_PORTS)
  {
    return -ERR_IPC_INVALID_PORT;
  }
  //return invalid msg size if req_size or rep_size is less than 1 or exceeds max msg size
  if ((req_size < 1) | (req_size >= COMMS_MAX_MSG) | (rep_size < 1) | (rep_size >= COMMS_MAX_MSG))
  {
    return -ERR_IPC_INVALID_MSG_SIZE;
  }
  //return invalid buffer when a buffer equals NULL
  if ((req_buf == NULL) | (rep_buf == NULL))
  {
    return -ERR_IPC_INVALID_BUFFER;
  }
  
  //=========================================SEND MSG=====================================
  //block sender
  //attempt to transfer msg to receiver
  //if busy wait
  //if received then unblock receiver
  msg *temp = (struct msg*)mem_alloc(sizeof(struct msg));
  if (msg_port[port]->queue == NULL)
  {
    msg_port[port]->queue = temp;
    msg_port[port]->last = temp;
  }else
  {
    msg_port[port]->last->next = temp;
    msg_port[port]->last = temp;
  }
  temp->pd = get_current()->pcb->pd;
  temp->req_buf = mem_alloc(req_size);
  copy_4(req_buf,temp->req_buf,req_size/4);
  temp->req_size = req_size;
  temp->rep_buf = rep_buf;//mem_alloc(rep_size);
  //copy_4(rep_buf,temp->rep_buf,rep_size/4);
  temp->rep_size = rep_size;
  printk("I GOT A MESSAGE %d\n",readb(req_buf));
  //return OK
  return OK;
    
}

int comms_receive(unsigned int port, void *buf, unsigned int size) {
  //return no such port when if the port doesn't exist
  if (bt(comms_bitmap,port))
  {
    return -ERR_IPC_NO_SUCH_PORT;
  }
  //return invalid port if port exceeds max ports
  if (port >= COMMS_MAX_PORTS)
  {
    return -ERR_IPC_INVALID_PORT;
  }
  //return invalid msg size if buf is less than 1 or exceeds max msg size
  if ((size < 1) | (size >= COMMS_MAX_MSG))
  {
    return -ERR_IPC_INVALID_MSG_SIZE;
  }
  //return invalid buffer when a buffer equals NULL
  if (buf == NULL)
  {
    return -ERR_IPC_INVALID_BUFFER;
  }
  
  //========================================WAIT or RECIEVE MSG==========================
  //block if no messages waiting
  //process message
  if (msg_port[port]->queue != NULL)
  {
    msg* temp = msg_port[port]->queue;    
    printk("WANT TO RECIEVE HERE\n");
    copy_4(temp->req_buf,buf,size/4);
  }
  //block if no outstanding message
  
  //return OK
  return OK;
}

int comms_reply(unsigned int port, void *buf, unsigned int size) {
  //return no such port when port doesn't exist
  if (bt(comms_bitmap,port))
  {
    return -ERR_IPC_NO_SUCH_PORT;
  }
  //return invalid port when port exceeds max ports
  if (port >= COMMS_MAX_PORTS)
  {
    return -ERR_IPC_INVALID_PORT;
  }
  //return invalid msg size if buf is less than 1 or exceeds max msg size
  if ((size < 1) | (size >= COMMS_MAX_MSG))
  {
    return -ERR_IPC_INVALID_MSG_SIZE;
  }
  //return invalid buffer when a buffer equals NULL
  if (buf == NULL)
  {
    return -ERR_IPC_INVALID_BUFFER;
  }
  
  
  //===========================================COMPLETE Communication cycle========================
  if (msg_port[port]->queue != NULL)
  {
    msg* temp = msg_port[port]->queue;
    msg_port[port]->queue = msg_port[port]->queue->next;
    if (msg_port[port] == NULL)
    {
      msg_port[port]->last = NULL;
    }
    i386_set_page_directory(temp->pd);
    printk("WANT TO REPLY HERE %d %d\n",readb(buf),readb(temp->rep_buf));
    copy_4(buf,temp->rep_buf,size/4);
    i386_set_page_directory(msg_port[port]->pd);
    printk("WANT TO REPLY HERE %d %d\n",readb(buf),readb(temp->rep_buf));
  }
  
  
  //return OK  
  return OK;
}

int comms_close_port(unsigned int port) {
  //return error no such port if port doesn't exist
  if (bt(comms_bitmap,port))
  {
    return -ERR_IPC_NO_SUCH_PORT;
  }
  //return invalid port if port < 0 or bigger than max ports
  if (port >= COMMS_MAX_PORTS)
  {
    return -ERR_IPC_INVALID_PORT;
  }
  
  //=========================================close port========================================
  //remove outstanding messages
  msg* temp = msg_port[port]->queue;
  while (temp != NULL)
  {
    msg_port[port]->queue = msg_port[port]->queue->next;
    mem_free(temp);
    temp = msg_port[port]->queue;
  }
  msg_port[port]->queue = NULL;
  msg_port[port]->last = NULL;
  
  //set port as free
  bts(comms_bitmap,port);
  
  //return OK
  return OK;
}
