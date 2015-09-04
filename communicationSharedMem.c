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

void sendData(Connection * connection, int size, void * params){
	enter(!bool_server);
		if(bool_server)
		msg=getmem(connection->sender_pid);
	else
		msg=getmem(0);

// memcpy(DEST; SOURCE; BYTES)
	Datagram * aux=params;
	printf("%i -> %i, %i\n", aux->opcode,aux->client_pid,size);
// TERMINA EL TEST DEL DATAGRAM

	memcpy(msg, params,size);
	printf("Paquete escrito en memoria por %i\n",aux->client_pid);
	leave(!bool_server);
}

void receiveData(Connection * sender, int size, void * buffer){

		if(bool_server)
			msg=getmem(0);
		else
			msg=getmem(sender->sender_pid);


		printf("Leyendo de memoria, bloqueante\n");

		char* current=calloc(size,0);

		while(strlen(current)==0){
			enter(bool_server);
			
			memcpy(current, msg);

			leave(bool_server);
		}
		

		
		printf("Recibo algo\n");

		memcpy(buffer,current,size);
		printf("salgo\n");
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
		strcpy(name,"mem_cli_");
		char* aux=name+8;
		while(mem_code!=0){
			*aux=mem_code%10+'0';
			mem_code/=10;
			aux++;
		}
		*aux=0;
	}

	if ( (fd = shm_open(name, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, SIZE);
	if ( !(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);

	printf("Memoria usada: %s\n",name);
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
