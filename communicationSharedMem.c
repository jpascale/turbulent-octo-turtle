#include "communication.h"
#include "datagram.h"
#include <stdio.h>
#include <string.h>

void sendData(Connection * connection, int size, void * params){

}


void receiveData(Connection * sender, int size, void * buffer){

}



#include "common.h"

int
main(int argc, char **argv)
{
	char *msg;
	char buf[SIZE-1];
	int n;
	
	msg = getmem();
	initmutex();
		
	while ( (n = read(0, buf, sizeof buf)) > 0 )
	{
		enter();
		sprintf(msg, "%.*s", n, buf);
		printf("Cliente escribe: %s", msg);
		leave();
	}
	printf("Cliente termina\n");
	return 0;
 }
