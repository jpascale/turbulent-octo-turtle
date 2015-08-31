#include "datagram.h"

void sendData(Connection * connection, int size, void * params);
void receiveData(Connection * sender, int size, void * buffer);
void initChannel(Connection * local);