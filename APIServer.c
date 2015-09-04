#include "sharedFunctions.h"
#include "communication.h"
#include "datagram.h"
#include <stdio.h>

Datagram data;
Connection sender;

void main(){
	
	initChannel(1);
	printf("Server conectado\n");


	printf("Cargado\n");
	while(1){
		receiveData(&sender, &data);
		
		printf("Recive:%i \n",data.client_pid);
		ProcessData(&sender, &data);
	}

}

void ProcessData(Connection * sender, Datagram * data){
	sleep(2);

	switch(data->opcode){
	
		case GET_MOVIE_LIST:
			printf("Llama consultar cartelera de server");
			break;
		
		default:
			printf("Comando no soportado");
	}
	sender->sender_pid=data->client_pid;
	printf("%i\n\n",sender->sender_pid);
	sendData(sender,data);
}
