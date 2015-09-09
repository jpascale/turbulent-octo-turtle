#include "communication.h"
#include "datagram.h"
#include "sharedFunctions.h"
#include "sqlite/sqlite3.h"
#include "sqlLib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ProcessData(Connection * sender, Datagram * data);

Datagram data;
Connection sender;


void main(){
	
	setUpDB();
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
	sleep(2);
	printf("FIN DEL SLEEP!\n");

	switch(data->opcode){
		
		case GET_MOVIE_LIST:
		printf("Llama consultar cartelera de server\n");
		break;
		default:
		printf("Comando no soportado\n");
	}
	char* hard_ans="NOMBRE1 ; ID1 ; NOMBRE2 ; ID2 ; NOMBRE3 ; ID3";
	char* lepoDice=malloc(9999);
	sender->sender_pid=data->client_pid;
	strcpy( data->data.text, hard_ans);
	SQLgetMovieList(lepoDice);
	printf("Lepo dice: %s\n",lepoDice);
	printf("Copiado: %s\n",hard_ans);
}
