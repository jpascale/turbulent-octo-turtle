#include "client.h"
#include "datagram.h"
#include "communication.h"

Datagram * data = malloc(sizeof(Data));
Connection * sender = malloc(sizeof(Connection));

void connect(int bool_server){
	initChannel(0);
	printf("Cliente conectado")
}

char * getMovieList(){
	data->opcode=CONSULTAR_CARTELERA;
	sender->pid=getpid();
	sendData(sender, sizeof(Datagram), data);
}

char * getMovieShow(int movieId){

}

char * getShowSeats(int showId){

}


int BuyTicket(int showId, int seat, int card, int sec_code, char * user_name){

}

int UndoBuyTicket(int ticketId, char * user_name){

}
