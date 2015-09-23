#ifndef _DATAGRAM_H
#define _DATAGRAM_H

#define MAX_STR_SIZE 1024

typedef enum {GET_MOVIE_LIST, GET_MOVIE_DETAILS, GET_MOVIE_SHOW, GET_SHOW_SEATS,
              BUY_TICKET, UNDO_BUY_TICKET, ADD_SHOW, REMOVE_SHOW, ADD_MOVIE, REMOVE_MOVIE
             } command;

typedef struct {
	int showId;
	int asiento;
	int tarjeta;
	int secCode;
	char nombre[MAX_STR_SIZE];
} buyStruct;

typedef struct {
	int length;
	char title[MAX_STR_SIZE];
	char desc[MAX_STR_SIZE];
} movieStruct;

typedef struct {
	int time;
	int roomId;
	int movieId;
} addShowStruct;

typedef struct {
	int ticketId;
	char nombre[MAX_STR_SIZE];
} undoBuyStruct;

typedef union {
	int i;
	char text[MAX_STR_SIZE];
	buyStruct buy;
	undoBuyStruct undoBuy;
	addShowStruct addShow;
	movieStruct movie;
} DataStruct;

typedef struct {
	int size;
	int opcode;
	int client_pid;
	DataStruct data; // tagUNION
} Datagram;

typedef struct {
	char title[MAX_STR_SIZE];
	char desc[MAX_STR_SIZE];
	int length;
	int movieId;
} Movie;

typedef enum {
	TO_SERVER, TO_CLIENT
} route;

typedef struct {
	int sender_pid;
} Connection;

typedef struct {
	int roomId;
	int max_cap;
} Room;

typedef struct {
	int showId;
	int time;
	int roomId;
	int movieId;
} Show;

typedef struct {
	int ticketId;
	int seatNum;
	int showId;
} Ticket;

#endif