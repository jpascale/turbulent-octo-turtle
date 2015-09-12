#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "datagram.h"
#include "sharedFunctions.h"
#define NAME "/tmp/fifo"
#define MAGIC_NUMBER_TO_AVOID_WARNINGS 12


char * getMovieList(){
	char* ans=malloc(1000);
	ans="NOMBRE1 ; ID1 ; NOMBRE2 ; ID2 ; NOMBRE3 ; ID3";
	return ans;
}

char * getMovieShow(int movieId){
	char* ans=malloc(1000);
	sprintf(ans, "Cadena de datos del movieId: %i generado en server\n",movieId);
	return ans;
}

char * getShowSeats(int showId){
	char* ans=malloc(1000);
	sprintf(ans, "Cadena de asientos del show: %i generado en server\n",showId);
	return ans;
}
// ret: ticketId
char * BuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){
	printf("ping\n");
	char* ans=malloc(1000);
	sprintf(ans, "Me dicen %i %i %i %i %s y yo les doy el ID 3\n",showId,  asiento,  tarjeta, secCode, nombre);	
	return ans;
}
// ret: confirmation code
char * UndoBuyTicket(int ticketId, char* nombre){

}
char * addShow(int time, int roomID, int movieID){

}
char * removeShow(int movieId){

}
char * addMovie(int length, char * title, char * desc){

}
char * removeMovie(int movieID){

}
