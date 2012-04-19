#ifndef CLNTSVR_H
#define CLNTSVR_H

#define PORT 10

typedef struct message {
  int x;
  int y;
  int z;
  char data[20480];
} message;

#endif
