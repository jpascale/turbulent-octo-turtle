#ifndef _DATAGRAM_H
#define _DATAGRAM_H

#define SIZE 1024

typedef enum {GET_MOVIE_LIST, GET_MOVIE_SHOW, GET_SHOW_SEATS,
BUY_TICKET, UNDO_BUY_TICKET} command;

typedef struct{
	char title[SIZE];
	char desc[SIZE];
	int length;
	int movieId;
} Movie;

typedef union{
	int i;
	Movie m;
}DataStruct;

typedef struct{
	int size;
	int opcode;
	int client_pid;
	DataStruct data; // tagUNION
} Datagram;

typedef enum {
	TO_SERVER, TO_CLIENT
} route;

typedef struct{
	int sender_pid;
}Connection;

typedef struct{
	char * full_name;
	int DNI;
	char * pass;
} User;

typedef struct{
	int roomId;
	int max_cap;
} Room;



typedef struct{
	int showId;
	char * time;
	int roomId;
	int movieId;
} Show;

typedef struct{
	int ticketId;
	int client_dni;
	int showId;
} Ticket;

#endif