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
	SQLgetMovieList(ans);
	return ans;
}

char * getMovieShow(int movieId){
	char* ans=malloc(1000);
	SQLgetMovieShow(ans,movieId);
	return ans;
}

char * getMovieDetails(int movieId){
	char* ans=malloc(1000);
	SQLgetMovieDetails(ans,movieId);
	return ans;
}

char * getShowSeats(int showId){
	char* ans=malloc(1000);
	SQLgetShowSeats(ans,showId);
	return ans;
}
// ret: ticketId
char * BuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){
	printf("ping\n");
	char* ans=malloc(1000);
	SQLbuyTicket(ans,showId,asiento,nombre);
	return ans;
}
// ret: confirmation code
char * UndoBuyTicket(int ticketId, char* nombre){
	char* ans=malloc(1000);
	SQLundoBuyTicket(ans,ticketId,nombre);
	return ans;
}
char * addShow(int time, int roomID, int movieID){
	char* ans=malloc(1000);
	SQLaddShow(ans,time,roomID,movieID);
	return ans;
}
char * removeShow(int showId){
	char* ans=malloc(1000);
	SQLremoveShow(ans,showId);
	return ans;
}
char * addMovie(int length, char * title, char * desc){
	char* ans=malloc(1000);
	SQLaddMovie(ans,length,title,desc);
	return ans;
}
char * removeMovie(int movieID){
	char* ans=malloc(1000);
	SQLremoveMovie(ans,movieID);
	return ans;
}
