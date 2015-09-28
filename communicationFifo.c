#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#include "./datagram.h"
#include "./sharedFunctions.h"
#include "communication.h"

#define SERVER_FIFO "/tmp/server.fifo"

int is_server;
int fd_read, fd_write, i;
char fileName[32];

/*	CLIENT:	Initializes server's fifo.
*	SERVER:	Initializes each client's fifo and opens server's fifo.
*/

void initChannel(int bool_server) {
	is_server = bool_server;
	if (is_server) {
		mknod(SERVER_FIFO, S_IFIFO | 0666, 0);
	} else {
		sprintf(fileName, "/tmp/fifo_cli%d", getpid());		
		mknod(fileName, S_IFIFO | 0666, 0);
		if(!access (SERVER_FIFO, F_OK)){
			printf("existe server.fifo en init\n");
			fd_write = open(SERVER_FIFO, O_WRONLY);
		}
		else{
			printf("no existe server.fifo en init\n");
			fd_write = -1;
		}
	}
}

/*	CLIENT:	Checks if a server is available, and writes on server's fifo.	
*	SERVER:	Creates client's fifo adn writes on it the response.
*/	
int sendData(Connection * connection, Datagram * params) {

	if (!is_server) {
		if(fd_write<0 || access(SERVER_FIFO, F_OK)){
			if(access(SERVER_FIFO, F_OK)){
				fd_write = -1;
				return -1;
			}
			else{
				fd_write = open(SERVER_FIFO, O_WRONLY);
			}
		}
		write(fd_write, params, *(int*)params);	
	} else {
		sprintf(fileName, "/tmp/fifo_cli%d", connection->sender_pid);
		fd_write = open(fileName, O_WRONLY);
		mknod(fileName, S_IFIFO | 0666, 0);
		write(fd_write, params, *(int*)params);
	}
	return 0;
}

/*	CLIENT:	Opens it own fifo, and reads the response from it.
*	SERVER:	Opens it own fifo, and reads the request from it.
*/
void receiveData(Connection * sender, Datagram * buffer) {

	if (!is_server) {
		sprintf(fileName, "/tmp/fifo_cli%d", *(((int*)buffer) + 2));
		fd_read = open(fileName, O_RDONLY);
	} else {
		fd_read = open(SERVER_FIFO, O_RDONLY);
	}
	while (!read(fd_read, buffer, sizeof(int)));
	int size = *((int*)buffer);
	read(fd_read, ((char*)buffer) + sizeof(int), size - sizeof(int));
	close(fd_read);
}

//	Removes fifos.

void handOff(int sig) {
	close(fd_read);
	close(fd_write);

	if(is_server){
		if(!remove(SERVER_FIFO))
			printf("Server stopped correctly\n");
		else
			printf("Server's communication file could not be removed\n");
	}else{
		if(!remove(fileName))
			printf("Client stopped correctly\n");
		else
			printf("Client's communication file could not be removed\n");	
	}
	exit(0);
}

