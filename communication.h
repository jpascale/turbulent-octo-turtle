#include "datagram.h"

void sendData(Connection * connection, Datagram * params);
void receiveData(Connection * sender, Datagram * buffer);
void initChannel(int bool_server);
