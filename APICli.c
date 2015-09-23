#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sharedFunctions.h"
#include "datagram.h"
#include "communication.h"

Datagram data;
Connection sender;

void __connect() {
	initChannel(0);
	printf("Cliente conectado.\n");
}

char * getMovieList() {

	data.opcode = GET_MOVIE_LIST;
	data.client_pid = getpid();
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}

char * getMovieShow(int movieId) {
	data.opcode = GET_MOVIE_SHOW;
	data.client_pid = getpid();
	data.data.i = movieId;
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}

char * getMovieDetails(int movieId) {
	data.opcode = GET_MOVIE_DETAILS;
	data.client_pid = getpid();
	data.data.i = movieId;
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}

char * getShowSeats(int showId) {
	data.opcode = GET_SHOW_SEATS;
	data.client_pid = getpid();
	data.data.i = showId;
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}


char * BuyTicket(int showId, int asiento, int tarjeta, int secCode, char* nombre) {
	data.opcode = BUY_TICKET;
	data.client_pid = getpid();
	data.data.buy.showId = showId;
	data.data.buy.asiento = asiento;
	data.data.buy.tarjeta = tarjeta;
	data.data.buy.secCode = secCode;
	memcpy(data.data.buy.nombre, nombre, 1024);
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;

}

char * UndoBuyTicket(int ticketId, char* nombre) {
	data.opcode = UNDO_BUY_TICKET;
	data.client_pid = getpid();
	data.data.undoBuy.ticketId = ticketId;
	memcpy(data.data.undoBuy.nombre, nombre, 1024);
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}


char * addShow(int time, int roomID, int movieID) {
	data.opcode = ADD_SHOW;
	data.client_pid = getpid();
	data.data.addShow.time = time;
	data.data.addShow.roomId = roomID;
	data.data.addShow.movieId = movieID;
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}

char * removeShow(int showId) {
	data.opcode = REMOVE_SHOW;
	data.client_pid = getpid();
	data.data.i = showId;
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}

char * addMovie(int length, char * title, char * desc) {
	data.opcode = ADD_MOVIE;
	data.client_pid = getpid();
	data.data.movie.length = length;
	memcpy(data.data.movie.title, title, 1024);
	memcpy(data.data.movie.desc, desc, 1024);
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;

}

char * removeMovie(int movieID) {
	data.opcode = REMOVE_MOVIE;
	data.client_pid = getpid();
	data.data.i = movieID;
	data.size = sizeof(data);
	sender.sender_pid = getpid();

	communicate(&sender, &data);

	return data.data.text;
}


void communicate(Connection * sender, Datagram * data) {
	if (sendData(sender, data) != -1)
		receiveData(sender, data);
	else {
		(*data).data.text[0] = 0;
	}
}