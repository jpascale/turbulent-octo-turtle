char * ConsultarCartelera();

char * ConsultarFunciones(int movieId);

char * ConsultarAsientos(int showId);

// ret: ticketId
char * ComprarTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre);

// ret: confirmation code
char * DeshacerCompra(int ticketId, char* nombre);

char * addFunction(char* function);

char * removeFunction(int movieId);

char * RespString(char * resp);

char * RespInt(int resp);
