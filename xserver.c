#include "datagram.h"
#include "communication.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int
main(int argc, char **argv)
{

	initChannel(1);
	
	Connection conn;
	Datagram data;

	while (1){
		receiveData(&conn, &data);
		sendData(&conn, &data);
	}
 }
