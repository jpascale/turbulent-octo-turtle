#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./datagram.h"
#include "./sharedFunctions.h"

#define SERVER_FIFO "/tmp/server.fifo"

// void sendData(Connection * connection, Datagram * params);
// void receiveData(Connection * sender, Datagram * buffer);
// void initChannel(int bool_server);

void mypause(int sign);

int is_server;
int fd_read, fd_write, i;
char fileName[32];

void initChannel(int bool_server) {
	is_server = bool_server;
	signal(SIGINT, handOff);
	if (is_server) {
		mknod(SERVER_FIFO, S_IFIFO | 0666, 0);
	} else {
		sprintf(fileName, "/tmp/fifo_cli%d", getpid());
		mknod(fileName, S_IFIFO | 0666, 0);
		fd_write = open(SERVER_FIFO, O_WRONLY);
	}
}

int sendData(Connection * connection, Datagram * params) {

	if (!is_server) {
		if(fd_write<0 && (fd_write = open(SERVER_FIFO, O_WRONLY)<0))
			return -1;
		mknod(SERVER_FIFO, S_IFIFO | 0666, 0);
		write(fd_write, params, *(int*)params);
		
	} else {
		sprintf(fileName, "/tmp/fifo_cli%d", connection->sender_pid);
		fd_write = open(fileName, O_WRONLY);
		mknod(fileName, S_IFIFO | 0666, 0);
		write(fd_write, params, *(int*)params);
	}
	return 0;
}

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

void handOff(int sig) {
	
	close(fd_read);
	close(fd_write);
	if(is_server){
		printf("Stopping server\n");
		if(!remove(SERVER_FIFO))
			printf("%s deleted\n", SERVER_FIFO);
		else
			printf("Couldn't delete %s\n", SERVER_FIFO);
	}else{
		printf("Stopping client\n");
		if(!remove(SERVER_FIFO))
			printf("%s deleted\n", fileName);
		else
			printf("Couldn't delete %s\n", fileName);
	}
	exit(0);
}

