#include "communication.h"
#include "datagram.h"
#include "common.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SIZE 1000

typedef enum { false, true } bool;

void fatal(char *s);
char *getmem(void);
void initmutex(void);
void enter(void);
void leave(void);

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

char *
getmem(void)
{
	int fd;
	char *mem;
	
	if ( (fd = shm_open("/message", O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, SIZE);
	if ( !(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);
	return mem;
}

static sem_t *sd;

void
initmutex(void)
{
	if ( !(sd = sem_open("/mutex", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
enter(void)
{
	sem_wait(sd);
}

void
leave(void)
{
	sem_post(sd);
}


void initChannel(Connection * local){
	
}

void sendData(Connection * connection, int size, void * params){

}

void receiveData(Connection * sender, int size, void * buffer){

}



//CLIENTE
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

 //SERVER
 int
main(int argc, char **argv)
{
	char *msg;
	static char buf[SIZE];
	int n;
	
	msg = getmem();
	memset(msg, 0, SIZE);
	initmutex();
		
	while ( true )
	{
		enter();
		if ( strcmp(buf, msg) )
		{
			printf("Servidor lee: %s", msg);
			strcpy(buf, msg);
		}
		leave();
		sleep(1);
	}
	return 0;
 }

