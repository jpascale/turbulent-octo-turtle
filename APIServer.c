#include "sharedFunctions.h"
#include "communication.h"
#include "datagram.h"
#include <stdio.h>
#include <unistd.h>

Datagram data;
Connection sender;

void main(){
	
	initChannel(1);
	printf("Server conectado\n");


	printf("Cargado\n");
	while(1){

		receiveData(&sender, &data);
		printf("Reccive:%i \n",data.client_pid);

		switch(int forked_pid=fork()){
			case -1:
				fatal("forked server\n");
			case 0:
				ProcessData(&sender, &data);
				sendData(&sender, &data);
				printf("TTERMINASDSADSAD_--------\n");
				exit(0);
			default:
				printf("Soy papa server y mi hijo %i responde a %i\n",forked_pid,data.opcode);
		}
	}
}

void ProcessData(Connection * sender, Datagram * data){
	sleep(3);
	printf("Opcode: %i\n",data->opcode);
	switch(data->opcode){
	
		case GET_MOVIE_LIST:
			printf("Llama consultar cartelera de server\n");
			break;
		default:
			printf("Comando no soportado\n");
	}
	sender->sender_pid=data->client_pid;
	data->size=3;
	printf("Le mando una respuesta a %i\n\n",sender->sender_pid);
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}
