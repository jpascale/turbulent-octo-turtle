#include "client.h"
#include "datagram.h"
#include "communication.h"

Datagram * data = malloc(sizeof(Data));
Connection * sender = malloc(sizeof(Connection));

char * ConsultarCartelera(){
	data->opcode=CONSULTAR_CARTELERA;
	sender->pid=getpid();
	sendData(sender, sizeof(Datagram), data);
}

char * ConsultarFunciones(int movieId){

}

char * ConsultarAsientos(int showId){

}


int ComprarTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){

}

int DeshacerCompra(int ticketId, char* nombre){

}
