#include "datagram.h"

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

void sendData(Connection * connection, Datagram * params);
void receiveData(Connection * sender, Datagram * buffer);
void initChannel(int bool_server);
void processData(Connection * sender, Datagram * data);

#endif