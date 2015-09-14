#include "communication.h"
#include "datagram.h"
#include "sharedFunctions.h"
#include "sqlite/sqlite3.h"
#include "sqlLib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void ProcessData(Connection * sender, Datagram * data);

Datagram data;
Connection sender;


void main(){

	initChannel(1);
	printf("Server conectado\n");
	setUpDB();
	
	int forked_pid, aux;

	printf("Cargado\n");
	char buffer[512], * auxString;

	SQLaddShow(buffer, 334, 2, 12);
	printf("resultado: %s\n", buffer);
	
	/*while(1){
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
	*/
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
}
