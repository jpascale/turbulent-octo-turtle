#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sharedFunctions.h"
#include "datagram.h"
#include "communication.h"

Datagram data;
Connection sender;


/*	Datagram datag;
	datag.size = 1;
	datag.opcode = 2;
	datag.client_pid = 3;

	datag.data.i = 4;

	strcpy(datag.data.m.title, "hola que tal");
	strcpy(datag.data.m.desc, "sadsdasda");
	datag.data.m.length = 5;
	datag.data.m.movieId = 6;



	printf("%i\n",sizeof(datag));
	printf("%c\n", ((char*)&datag)[12]);
	
	void * datag2 = malloc(2068);

	memcpy(datag2, &datag, 2068);

	Datagram * datag3 = (Datagram * ) datag2;
	printf("%c\n", ((char*)datag3)[12]);
*/	


void connect(){
	initChannel(0);
	
	printf("Cliente conectado.\n");
}

char * getMovieList(){

	printf("Entrando a APLCli.\n");
	
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
