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
	char msg[200];
	int n;
	Datagram data;
	Connection conn;

	initChannel(0);

	while ( (n = read(0, data.data.m.title, sizeof data.data.m.title)) > 0 )
	{
		data.size = n;

		printf("Cliente envia: %.*s", n, data.data.m.title);
		sendData(&conn, &data);
	}
	printf("Cliente termina\n");
	return 0;
 }
