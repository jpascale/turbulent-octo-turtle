#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./datagram.h"

#define NAME "/tmp/fifo"

void sendData(Connection * connection, int size, void * params);
void receiveData(Connection * sender, int size, void * buffer);
void initChannel(int bool_server);

void getFifoName(int pid, char * buffer);

int is_server;

void initChannel(int bool_server){
	is_server = bool_server;
}

int fd, i;

void sendData(Connection * connection, int size, void * params){
	if (!is_server){
		fd = open("/tmp/server.fifo", O_WRONLY);
		mknod("/tmp/server.fifo", S_IFIFO|0666, 0);
		write(fd, "mensaje", 7);
	}
}

void receiveData(Connection * sender, int size, void * buffer){
	fd = open("/tmp/server.fifo", O_RDONLY); 
	if(is_server)
		fd = open("/tmp/server.fifo", O_RDONLY); 
	read(fd, buffer, 1000);	
}

void getFifoName(int pid, char * buffer){
	while(pid!=0){
		*buffer = pid%10 + '0';
		pid/=10;
		buffer++;
	}
	*buffer=0;
}
