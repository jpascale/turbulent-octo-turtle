#include "sharedFunctions.h"
#include "communication.h"
#include "datagram.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

Datagram data;
Connection sender;


void ProcessData(Connection * sender, Datagram * data);

void main(){
	
	initChannel(1);
	printf("Server conectado\n");

	int forked_pid;

	printf("Cargado\n");
	while(1){
		receiveData(&sender, &data);
		printf("Recive:%i \n",data.client_pid);

		switch(forked_pid=fork()){
			case -1:
				perror("forked server\n");
				exit(1);
			case 0:
				ProcessData(&sender, &data);
				sendData(&sender, &data);
				printf("Respuesta enviada\n");
				exit(0);
		}
	}
}

void ProcessData(Connection * sender, Datagram * data){
	printf("Entro al sleep. 'ATIENDE SERVER...'\n");
	sleep(1);
	printf("LISTO!\n");
	switch(data->opcode){
	
		case GET_MOVIE_LIST:
			printf("Llama consultar cartelera de server\n");
			break;
		default:
			printf("Comando no soportado\n");
	}
	sender->sender_pid=data->client_pid;
	data->size=3;
}