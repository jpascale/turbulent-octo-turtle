char * getMovieList();

char * getMovieShow(int movieId);

char * getShowSeats(int showId);

// ret: ticketId
char * BuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre);

// ret: confirmation code
char * UndoBuyTicket(int ticketId, char* nombre);

char * addFunction(char* function);

char * removeFunction(int movieId);

char * RespString(char * resp);

char * RespInt(int resp);
