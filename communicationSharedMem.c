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
#define CLIENT_CANAL_PREFIX "mem_client"
#define SERVER_CANAL "mem_server"
typedef enum { false, true } bool;

void resetSems(void);
void fatal(char *s);
char * getmem(int mem_code);
void initmutex(void);
void enter(int code);
void leave(int code);
void printfSemStates(void);

static int bool_server;
char *msg;
static char buf[SIZE];
int n;
char* current;
static sem_t *sdA;
static sem_t *sdB;
static sem_t *sdC;

void initChannel(int b_server) {
	bool_server = b_server;
	initmutex();
	resetSems();
	memset(buf, 0, SIZE);
	current = calloc(10000, 1);
}

int sendData(Connection * connection, Datagram * params) {
//	printfSemStates();
	if (bool_server)
		msg = getmem(connection->sender_pid);
	else
		msg = getmem(0);
	if (msg == -1)
		return -1;

	if (!bool_server)
		enter(1);
	memcpy(msg, params, params->size);
	leave((bool_server) ? 2 : 0);
//	printf("Paquete escrito en memoria por %i de size %i\n",params->client_pid, params->size);
	return 0;
}

void receiveData(Connection * sender, Datagram * buffer) {
	// Si no estaba abierto el canal al server, no hay nada que leer =/
	if (bool_server)
		msg = getmem(0);
	else
		msg = getmem(sender->sender_pid);
//		printf("Leyendo de memoria, bloqueante\n");
	enter((bool_server) ? 0 : 2);
//		printf("rin, %i\n",getpid());
	memcpy(current, msg, sizeof(int));
//		printf("Busco un mensaje de size= %i\n",*((int*)current));
	memcpy(current, msg, *((int*)current));
	sprintf(msg, "\0\0\0\0");
	if (!bool_server)
		leave(1);
//		printf("Recibido\n");
	memcpy(buffer, current, *((int*)current));
//		printfSemStates();
	if (!bool_server)
		resetSems();
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
	char name[16];
	char error_flag = 0;
	if (mem_code == 0) {
		strcpy(name, SERVER_CANAL);
	} else {
		sprintf(name, "%s%i", CLIENT_CANAL_PREFIX, mem_code);
	}

	if ( (fd = shm_open(name, O_RDWR, 0666)) == -1 ) {
		if (mem_code == 0) {
			if (bool_server == 0) {
				printf("El server no esta corriendo\n");
				error_flag = 1;
			} else {
				if ((fd = shm_open(name, O_RDWR | O_CREAT, 0666)) == -1 ) {
					fatal("sh_open");
				}
			}
		} else {
			if (bool_server != 0) {
				printf("El cleinte no abrio canal de vuelta. ¿Tocaste el cliente?\n");
				error_flag = 1;
			} else {
				if ( (fd = shm_open(name, O_RDWR | O_CREAT, 0666)) == -1 ) {
					fatal("sh_open");
				}
			}
		}
	}
	if (!error_flag) {
		ftruncate(fd, SIZE);
		if ( !(mem = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) )
			fatal("mmap");
		close(fd);
	}
	return (error_flag == 1) ? -1 : mem;
}



void
initmutex(void)
{
	if ( !(sdA = sem_open("mutex_A", O_RDWR | O_CREAT, 0666, 1)) )
		fatal("sem_open");

	if ( !(sdB = sem_open("mutex_B", O_RDWR | O_CREAT, 0666, 1)) )
		fatal("sem_open");

	if ( !(sdC = sem_open("mutex_C", O_RDWR | O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
enter(int code)
{
	switch (code) {
	case 0:
		sem_wait(sdA);
		break;
	case 1:
		sem_wait(sdB);
		break;
	case 2:
		sem_wait(sdC);
		break;
	}
}

void
leave(int code)
{
	switch (code) {
	case 0:
		sem_post(sdA);
		break;
	case 1:
		sem_post(sdB);
		break;
	case 2:
		sem_post(sdC);
		break;
	}
}

void
printfSemStates() {
	int a, b, c;
	sem_getvalue(sdA, &a);
	sem_getvalue(sdB, &b);
	sem_getvalue(sdC, &c);
	printf("semA: %i,semB: %i,semC: %i\n", a, b, c);
}


// Resetea el estado de los semáforos
void
resetSems() {
	int value;
	sem_getvalue(sdA, &value);
	while (value > 0) {
		sem_wait(sdA);
		sem_getvalue(sdA, &value);
	}
	sem_getvalue(sdC, &value);
	while (value > 0) {
		sem_wait(sdC);
		sem_getvalue(sdC, &value);
	}
	sem_getvalue(sdB, &value);
	while (value > 1) {
		sem_wait(sdC);
		sem_getvalue(sdC, &value);
	}
	while (value < 1) {
		sem_post(sdC);
		sem_getvalue(sdC, &value);
	}
}

void handOff(int sig) {
	char name[16];

// Cierro los semaforos. Cuando no los use nadie, se borran.
	sem_close(sdA);
	sem_close(sdB);
	sem_close(sdC);

//Cada parte se encarga de cerrar sus canales

	if (bool_server) {
		msg = getmem(0);
		strcpy(name, SERVER_CANAL);
	} else {
		msg = getmem(getpid());
		sprintf(name, "%s%i", CLIENT_CANAL_PREFIX, getpid());
	}
	shm_unlink(name);

	shmdt(msg);
	printf("Termina por señal %d\n", sig);
	exit(0);
}