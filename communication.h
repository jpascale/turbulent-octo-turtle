#include "datagram.h"

#ifndef _COMM_

#define _COMM_

void sendData(Connection * connection, int size, void * params);
void receiveData(Connection * sender, int size, void * buffer);
void initChannel(int bool_server);

#endif