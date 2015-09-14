void setUpDB();

static int callback(void *answer, int argc, char **argv, char **azColName);

void SQLgetMovieList(char * buffer);
void SQLgetMovieDetails(char * buffer, int movieID);
void SQLgetMovieShow(char * buffer, int movieID);
void SQLgetShowSeats(char * buffer, int showID);
// ret: ticketId
void SQLbuyTicket(char * buffer, int showID, int asiento, char* nombre);
// ret: confirmation code
void SQLundoBuyTicket(char * buffer, int ticketID, char* nombre);
void SQLaddShow(char * buffer, int time, int roomID, int movieID);
void SQLremoveShow(char * buffer, int showID);

void SQLaddMovie(char * buffer, int length, char * title, char * desc);
void SQLremoveMovie(char * buffer, int movieID);