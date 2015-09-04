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

	printf("Entrando a APLCli \n");
	
	data.size = 1;
	data.opcode = 2;
	data.client_pid = 3;
	data.data.i = 9;

	//Buffer goes empty
	sendData(&sender, &data);

	receiveData(&sender, &data);

	printf("Data cruda, sin unmarshall recivida: \n",data);
	
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
