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
	data.opcode = 0;
	data.client_pid = getpid();
	data.size = sizeof(data);
	sender.sender_pid=getpid();

	sendData(&sender, &data);
	receiveData(&sender, &data);

	return data.data.text;
}

char * getMovieShow(int movieId){
	printf("movieId: %i\n",movieId);
}

char * getShowSeats(int showId){
	printf("showId: %i\n",showId);
}


char * BuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){
	printf("showId:%i,asiento: %i, tarjeta: %i, secCode: %i, nombre: %s \n",showId,  asiento,  tarjeta, secCode, nombre);
}

char * UndoBuyTicket(int ticketId, char* nombre){
	printf("ticketId: %i, nombre: %s\n",ticketId, nombre);
}


char * addShow(char* function){
	printf("E - %s\n",function);
}

char * removeShow(int movieId){
	printf("movieId: %i \n",movieId);

}