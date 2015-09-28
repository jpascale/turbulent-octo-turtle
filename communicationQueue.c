
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/stat.h>
/*
**		communicationQueue.c
*/

#include "datagram.h"
#include "communication.h"

/*
**			Private function declares
*/
void srv_init_channel(void);
void clt_init_channel(void);
void create_ioqueue(void);
int srv_send_data(Connection * connection, Datagram * sdData);
void srv_receive_data(Connection * connection, Datagram * sdData);
int clt_send_data(Connection * connection, Datagram * sdData);
void clt_receive_data(Connection * connection, Datagram * sdData);
void quit(int sig);
void fatal(char * s);

/*
**		Defines
*/

typedef enum { false, true } bool;
#define MAX_RDATA_SIZE 3000

/*
**		Global declares
*/
bool is_server;

key_t keyin;
key_t keyout;

int qin, qout;
ssize_t n;

struct
{
	long mtype;
	char mdata[MAX_RDATA_SIZE];
}
msg;

/*
**		Module functions
*/

/*
**	Init the communication channel
**	Receives 1 for server and 0 for client
*/
void initChannel(int bool_server) {

	switch (bool_server) {

	case true:
		srv_init_channel();
		break;

	case false:
		clt_init_channel();
		break;
	}
}

int sendData(Connection * connection, Datagram * params) {
	switch (is_server) {
	case true:
		return srv_send_data(connection, params);
		break;

	case false:
		return clt_send_data(connection, params);
		break;
	}
}

void receiveData(Connection * connection, Datagram * params) {
	switch (is_server) {
	case true:
		srv_receive_data(connection, params);
		break;

	case false:
		clt_receive_data(connection, params);
		break;
	}
}


/*
**		Private module functions
*/

/*
**      srv_init_channel: Load the in/out key
*/
void srv_init_channel(void) {
	is_server = true;

	keyin = 0xBEEF0;
	keyout = 0xBEEF1;

	create_ioqueue();
}

/*
**      clt_init_channel: Load the in/out key
*/
void clt_init_channel(void) {
	is_server = false;

	keyin = 0xBEEF1;
	keyout = 0xBEEF0;

	create_ioqueue();
}

/*
**      create_ioqueue: create in/out queue
*/
void create_ioqueue(void) {

	if ( (qin = msgget(keyin, 0666 | IPC_CREAT)) == -1 )
		fatal("Error msgget qin");

	if ( (qout = msgget(keyout, 0666 | IPC_CREAT)) == -1 )
		fatal("Error msgget qout");

	return;

}

/*
**      srv_send_data: copy params into a buffer and send them to out queue
*/
int srv_send_data(Connection * connection, Datagram * sdData) {

	n = sdData->size;
	msg.mtype = sdData->client_pid;
	memcpy((void*)msg.mdata, (void*)sdData, sdData->size);

	msgsnd(qout, &msg, n, 0);
	
	//Reset bff
	int i;
	for (i = 0; i < 1024; i++)
		*(msg.mdata + 12 + i) = '\0';

	return 0;

}

/*
**      srv_receive_data: Receive data from the in queue,
**		copy buffer to data and return it.
*/
void srv_receive_data(Connection * connection, Datagram * sdData) {

	void * rdata = (void *)sdData;

	if ( (n = msgrcv(qin, &msg, sizeof msg.mdata, 0, 0)) > 0 )
	{
		printf("Servidor: %s", msg.mdata + 12);

		memcpy(rdata, msg.mdata, n);

		return;
	}
}

/*
**      clt_send_data: Load current process pid into package,
**		copy data to buffer and send it through out queue.
*/
int clt_send_data(Connection * connection, Datagram * sdData) {

	msg.mtype = getpid();
	n = sdData->size;

	void * sdDataptr = (void *) sdData;

	memcpy((void *)msg.mdata, sdData, n);

	return msgsnd(qout, &msg, n, 0);

}

/*
**      clt_receive_data: Read from in queue, copy buffer to data
**		and return it.
*/
void clt_receive_data(Connection * connection, Datagram * sdData) {

	n = msgrcv(qin, &msg, sizeof msg.mdata, msg.mtype, 0);

	memcpy((void * )sdData, (void *)msg.mdata, *((int *)msg.mdata));

	int i;
	for (i = 0; i < 1024; i++)
		*(msg.mdata + 12 + i) = '\0';
}

void fatal(char * s) {
	perror(s);
	exit(1);
}

void handOff(int sig) {
	printf("Termina por señal %d\n", sig);
	exit(0);
}