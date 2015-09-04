#include "communication.h"
#include "datagram.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 1000

typedef enum { false, true } bool;

void fatal(char *s);
char * getmem(int mem_code);
void initmutex(void);
void enter(int bool_cs);
void leave(int bool_cs);

static int bool_server;
char *msg;
static char buf[SIZE];
int n;

void initChannel(int b_server){
	bool_server=b_server;
	initmutex();	
	memset(buf, 0, SIZE);
}

void sendData(Connection * connection, Datagram * params){
	enter(!bool_server);
	if(bool_server)
		msg=getmem(connection->sender_pid);
	else
		msg=getmem(0);

	memcpy(msg, params,params->size);
//	printf("Paquete escrito en memoria por %i de size %i\n",params->client_pid, params->size);
	leave(!bool_server);
}

void receiveData(Connection * sender, Datagram * buffer){

		if(bool_server)
			msg=getmem(0);
		else
			msg=getmem(sender->sender_pid);


//		printf("Leyendo de memoria, bloqueante\n");

		char* current=calloc(10000,1);

		while(strlen(current)==0){
			enter(bool_server);
			
			memcpy(current, msg,sizeof(int));

			leave(bool_server);
		}
//		printf("Busco un mensaje de size= %i\n",*((int*)current));

		memcpy(current, msg,*((int*)current));
	
//		printf("Recibido\n");

		memcpy(buffer,current,SIZE);
		sprintf(msg,"\0\0\0\0");
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

char *
getmem(int mem_code)
{
	int fd;
	char * mem;
	char * name= malloc(16);

	if(mem_code == 0){
		strcpy(name,"mem_cliServ");
	}else{
		sprintf(name,"mem_cli%i",mem_code);
	}

	if ( (fd = shm_open(name, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, SIZE);
	if ( !(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);

//	printf("Memoria pedida: %s\n",name);
	return mem;
}


static sem_t *sdcs;

static sem_t *sdsc;

void
initmutex(void)
{
	if ( !(sdcs = sem_open("mutex_cs", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");

	if ( !(sdsc = sem_open("mutex_sc", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
enter(int bool_cs)
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
