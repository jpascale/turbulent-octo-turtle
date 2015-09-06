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

typedef enum { false, true } bool;

int
main(int argc, char **argv)
{
	int n;
	Datagram data;
	Connection conn;

	initChannel(0);

	while ( (n = read(0, data.data.m.title, sizeof data.data.m.title)) > 0 )
	{
		data.size = sizeof data;

		printf("Cliente envia: %.*s", n, data.data.m.title);
		sendData(&conn, &data);
		receiveData(&conn, &data);
		
		//TODO: Remove this
		int i;
		for (i = 0; i < 1024; i++)
			data.data.m.title[i] = '\0';
	}
	printf("Cliente termina\n");
	return 0;
 }
