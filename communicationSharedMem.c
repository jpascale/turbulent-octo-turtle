#include "communication.h"
#include "datagram.h"
//#include <common.h>

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
char * getmem(int bool_cs);
void initmutex(void);
void enter(int bool_cs);
void leave(int bool_cs);


static int bool_server;
char *msg;
static char buf[SIZE];
int n;

void initChannel(int b_server){
	bool_server=b_server;
	msg = getmem();
	initmutex();
	if(b_server)
			memset(msg, 0, SIZE);

}

void sendData(Connection * connection, int size, void * params){
		
		enter(!bool_server);

		msg=getmem(!bool_server);
		sprintf(msg, "%.*s", size, params);
		printf("Mando algo");
	
		leave(!bool_server);
}

void receiveData(Connection * sender, int size, void * buffer){

		enter(bool_server);

		msg=getmem(bool_server);
		sprintf(msg, "%.*s", size, buffer);
		printf("Recibo algo, %s",buffer);
	
		leave(bool_server);

}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

char *
getmem(int bool_cs)
{
	int fd;
	char *mem;
	
	char * name[16];
	strcpy(name,"/message");
	char var=bool_cs+'0';
	strcat(name,&var);

	if ( (fd = shm_open(name, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, SIZE);
	if ( !(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);
	return mem;
}

static sem_t *sdcs;

static sem_t *sdsc;

void
initmutex(void)
{


	if ( !(sdcs = sem_open("/mutexcs", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");

	if ( !(sdsc = sem_open("/mutexsc", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
enter(bool_cs)
{
	if(bool_cs)
		sem_wait(sdcs);
	else
		sem_wait(sdsc);
}

void
leave(int bool_cs)
{
	if(bool_cs)
		sem_post(sdcs);
	else
		sem_post(sdsc);
}