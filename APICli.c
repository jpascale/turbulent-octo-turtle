#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sharedFunctions.h"
#include "datagram.h"
#include "communication.h"

Datagram data;
Connection sender;


void connect(){
	initChannel(0);
	
	printf("Cliente conectado.\n");
}

char * getMovieList(){

	printf("Entrando a APLCli. Dame el opcode =) \n");
	
	data.opcode = 0;

	data.client_pid = getpid();
	data.size = sizeof(data);

	sender.sender_pid=getpid();

	sendData(&sender, &data);

	receiveData(&sender, &data);

	printf("Data cruda, sin unmarshall recibida: \n",data);
	
// ACA VA EL UNMARSHALL!!!!

	return 0;
}

char * getMovieShow(int movieId){

}

char * getShowSeats(int showId){

}


char * BuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){

}

char * UndoBuyTicket(int ticketId, char* nombre){

}
