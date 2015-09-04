#include <stdio.h>
#include <stdlib.h>
#include "sharedFunctions.h"
#include "communication.h"
#include "datagram.h"

#define MAGIC_NUMBER_TO_AVOID_WARNINGS 12


void main(){
	
	
	initChannel(1);
	printf("Server conectado\n");

	Connection * sender = malloc(sizeof(Connection));
	
	int pid, n;

	connect();
	
	Connection * connection;
	Datagram * data = malloc(1000);
	
	while(7 == 7){ //arreglar
		receiveData(connection, MAGIC_NUMBER_TO_AVOID_WARNINGS, data);	
		printf("%s", data);
	}
	

}

void ProcessData(Connection * sender, Datagram * data){
	switch(data->opcode){
	
		case GET_MOVIE_LIST:
			printf("Llama consultar cartelera de server");
			break;
		
		default:
			printf("Comando no soportado");
	}
}
