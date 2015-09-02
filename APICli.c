#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sharedFunctions.h"
#include "datagram.h"
#include "communication.h"

static Datagram * data;
static Connection * sender;


void connect(){
	initChannel(0);
	data = malloc(sizeof(Datagram));
	sender = malloc(sizeof(Connection));

	printf("Cliente conectado.\n");
}

char * getMovieList(){
	printf("Entrando a APLCli \n");
	sender->sender_pid=getpid();
	data->opcode=GET_MOVIE_LIST;
	

	//Buffer goes empty
	sendData(sender, sizeof(Datagram), data);

	receiveData(sender, sizeof(Datagram), data);

	printf("Data cruda, sin unmarshall %s\n",data);
	return data;
}

char * getMovieShow(int movieId){

}

char * getShowSeats(int showId){

}


char * BuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){

}

char * UndoBuyTicket(int ticketId, char* nombre){

}
