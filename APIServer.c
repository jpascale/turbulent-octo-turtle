#include "server.h"
#include "client.h"
#include "communication.h"
#include "datagram.h"

void main(){
	
	printf("Levantando server");

	Datagram * data = malloc(sizeof(Data));
	Connection * sender = malloc(sizeof(Connection));
	
	while(1){
		receiveData(sender, sizeof(Datagram), data);
		ProcessData(sender, data);
	}
}

void ProcessData(Connection * sender, Datagram * data){
	switch(data->opcode){
	case CONSULTAR_CARTELERA:
		prinf("Llama consultar cartelera de server");
		break;
	default:
		prinf("Comando no soportado");
	}
}
