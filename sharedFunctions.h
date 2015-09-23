#ifndef _SHARED_FUNCTIONS_H
#define _SHARED_FUNCTIONS_H

char * getMovieList();

char * getMovieShow(int movieId);

char * getMovieDetails(int movieId);

char * getShowSeats(int showId);

// ret: ticketId
char * BuyTicket(int showId, int asiento, int tarjeta, int secCode, char* nombre);

// ret: confirmation code
char * UndoBuyTicket(int ticketId, char* nombre);

char * addShow(int time, int roomID, int movieID);

char * removeShow(int showId);

char * addMovie(int length, char * title, char * desc);

char * removeMovie(int movieID);

void __connect(void);

void handOff(int sig);

#endif
