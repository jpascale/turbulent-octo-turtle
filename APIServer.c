#include "sharedFunctions.h"
#include "communication.h"
#include "datagram.h"
#include <stdio.h>

void main(){
	
	initChannel(1);
	printf("Server conectado\n");

	Datagram * data = malloc(sizeof(Datagram));
	Connection * sender = malloc(sizeof(Connection));
	printf("Cargado\n");
	while(1){
		receiveData(sender, sizeof(Datagram), data);
		ProcessData(sender, data);
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
	sendData(sender,sizeof(Datagram),data);
}
