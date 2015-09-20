#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./datagram.h"

#define NAME "/tmp/fifo"

void sendData(Connection * connection, Datagram * params);
void receiveData(Connection * sender, Datagram * buffer);
void initChannel(int bool_server);
void getFifoName(int pid, char * buffer);

int is_server;
int fd, i;
char fileName[32];

void initChannel(int bool_server){
	is_server = bool_server;
	if(bool_server){
		mknod("/tmp/server.fifo", S_IFIFO|0666, 0);
		printf("hasta aca server\n");
	}else{
		sprintf(fileName, "/tmp/fifo_cli%d", getpid());
		mknod(fileName, S_IFIFO|0666, 0);
		printf("hasta aca client\n");
	}
}

void sendData(Connection * connection, Datagram * params){
	
	if (!is_server){
		fd = open("/tmp/server.fifo", O_WRONLY);
		mknod("/tmp/server.fifo", S_IFIFO|0666, 0);
		write(fd, params, *(int*)params);
	}else{
		sprintf(fileName, "/tmp/fifo_cli%d", connection->sender_pid);
		fd = open(fileName, O_WRONLY);
		mknod(fileName, S_IFIFO|0666, 0);
		write(fd, params, *(int*)params);
	}
}

void receiveData(Connection * sender, Datagram * buffer){
	
	if(is_server){
		fd = open("/tmp/server.fifo", O_RDONLY); 
	}else{
		sprintf(fileName, "/tmp/fifo_cli%d", *(((int*)buffer)+2));
		fd = open(fileName, O_RDONLY);	
	}
	
	read(fd, buffer, sizeof(int));
	printf("esto leyo: %i\n", (int*)buffer);
	int size = *((int*)buffer);
	read(fd, ((char*)buffer)+sizeof(int), size - sizeof(int));
	
}

void getFifoName(int pid, char * buffer){
	while(pid!=0){
		*buffer = pid%10 + '0';
		pid/=10;
		buffer++;
	}
	*buffer=0;
}

void handOff(int sig){
	printf("Servidor termina por señal %d\n", sig);
	exit(0);
}
