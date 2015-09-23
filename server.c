#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "datagram.h"
#include "sharedFunctions.h"
#include "sqlLib.h"


char* ans;

void initializeServer() {
	ans = malloc(1024);
}

char * getMovieList() {
	SQLgetMovieList(ans);
	return ans;
}

char * getMovieShow(int movieId) {
	SQLgetMovieShow(ans, movieId);
	return ans;
}

char * getMovieDetails(int movieId) {
	SQLgetMovieDetails(ans, movieId);
	return ans;
}

char * getShowSeats(int showId) {
	SQLgetShowSeats(ans, showId);
	return ans;
}
// ret: ticketId
char * BuyTicket(int showId, int asiento, int tarjeta, int secCode, char* nombre) {
	SQLbuyTicket(ans, showId, asiento, nombre);
	return ans;
}
// ret: confirmation code
char * UndoBuyTicket(int ticketId, char* nombre) {
	SQLundoBuyTicket(ans, ticketId, nombre);
	return ans;
}
char * addShow(int time, int roomID, int movieID) {
	SQLaddShow(ans, time, roomID, movieID);
	return ans;
}
char * removeShow(int showId) {
	SQLremoveShow(ans, showId);
	return ans;
}
char * addMovie(int length, char * title, char * desc) {
	SQLaddMovie(ans, length, title, desc);
	return ans;
}
char * removeMovie(int movieID) {
	SQLremoveMovie(ans, movieID);
	return ans;
}
