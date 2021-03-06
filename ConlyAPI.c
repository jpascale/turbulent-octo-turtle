#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "datagram.h"
#include "sharedFunctions.h"
#include "sqlLib.h"

#define SLEEP_TIME 2
#define DB_PATH "test.db"
#define READ 1
#define WRITE 0

#define BUFFER_SIZE 1014

int fd;
void lock_db(int is_reader);
void unlock_db(void);
void simulateDelay();
char locked = 0;
char* ans;


/*	Initilizes the data base and the buffer to store answers from it.
*/
void __connect() {
	ans = calloc(BUFFER_SIZE, 1);
	setUpDB();
}

char * getMovieList() {	
	lock_db(READ);
	simulateDelay();
	SQLgetMovieList(ans);
	unlock_db();
	return ans;
}

char * getMovieShow(int movieId) {
	lock_db(READ);
	simulateDelay();
	SQLgetMovieShow(ans, movieId);
	unlock_db();
	return ans;
}

char * getMovieDetails(int movieId) {
	lock_db(READ);
	simulateDelay();
	SQLgetMovieDetails(ans, movieId);
	unlock_db();
	return ans;
}

char * getShowSeats(int showId) {
	lock_db(READ);
	simulateDelay();
	SQLgetShowSeats(ans, showId);
	unlock_db();
	return ans;
}
// ret: ticketId
char * BuyTicket(int showId, int asiento, int tarjeta, int secCode, char* nombre) {
	lock_db(WRITE);
	simulateDelay();
	SQLbuyTicket(ans, showId, asiento, nombre);
	unlock_db();
	return ans;
}
// ret: confirmation code
char * UndoBuyTicket(int ticketId, char* nombre) {
	lock_db(WRITE);
	simulateDelay();
	SQLundoBuyTicket(ans, ticketId, nombre);
	unlock_db();
	return ans;
}
char * addShow(int time, int roomID, int movieID) {
	lock_db(WRITE);
	simulateDelay();
	SQLaddShow(ans, time, roomID, movieID);
	unlock_db();
	return ans;
}
char * removeShow(int showId) {
	lock_db(WRITE);
	simulateDelay();
	SQLremoveShow(ans, showId);
	unlock_db();
	return ans;
}
char * addMovie(int length, char * title, char * desc) {
	lock_db(WRITE);
	simulateDelay();
	SQLaddMovie(ans, length, title, desc);
	unlock_db();
	return ans;
}
char * removeMovie(int movieID) {
	lock_db(WRITE);
	simulateDelay();
	SQLremoveMovie(ans, movieID);
	unlock_db();
	return ans;
}

/*	Frees the buffer allocated before and removes the lock from the database file if exists.
*/

void handOff(int sig) {
	free(ans);
	closeDatabase();

	if (locked) {
		unlock_db();
		printf("Quita el lock por signal\n");
	} else {
		printf("No estaba lockeado cuando llego la signal\n");
	}
}

/*	Sets a lock in the database file as reader or writer.	
*	If it's not possible, the function blocks the procces.
*/
void lock_db(int is_reader) {
	struct flock fl = {F_WRLCK, SEEK_SET,   0,      0,     0 };
	fl.l_pid = getpid();

	printf("Lockeando como escritura\n");
	if (is_reader) {
		fl.l_type = F_RDLCK;
		printf("Cambiado a lectura\n");
	}
	if ((fd = open(DB_PATH, O_RDWR)) == -1) {
		perror("open");
		exit(1);
	}
	printf("Trying to get lock...");

	if (fcntl(fd, F_SETLKW, &fl) == -1) {
		perror("fcntl");
		exit(1);
	}

	printf("got lock\n");
	locked = 1;
}

/*	Removes the lock from the database file.
*/
void unlock_db(void) {
	struct flock fl = {F_UNLCK, SEEK_SET,   0,      0,     0 };
	fl.l_pid = getpid();

	if (fcntl(fd, F_SETLKW, &fl) == -1) {
		perror("fcntl");
		exit(1);
	}

	printf("Unlocked.\n");
	locked = 0;
	close(fd);
}

void simulateDelay() {
	printf("Entro al sleep.\n");
	sleep(SLEEP_TIME);
	printf("FIN DEL SLEEP!\n");
}