#include "datagram.h"

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

void initChannel(int bool_server);
int sendData(Connection * connection, Datagram * params);
void receiveData(Connection * sender, Datagram * buffer);
void communicate(Connection * sender, Datagram * data);
void processData(Connection * sender, Datagram * data);

#endif