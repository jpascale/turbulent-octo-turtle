#include "communication.h"
#include "datagram.h"
#include "sharedFunctions.h"
#include "sqlite/sqlite3.h"
#include "sqlLib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void ProcessData(Connection * sender, Datagram * data);
void initializeServer();
Datagram data;
Connection sender;


void main(){
	signal(SIGINT, handOff);
	setUpDB();
	initChannel(1);
	printf("Server conectado\n");
	initializeServer();
	int forked_pid, aux;

	printf("Cargado\n");
	char buffer[512], * auxString;

	while(1){
		receiveData(&sender, &data);
		printf("Recive:%i \n",data.client_pid);

		switch(forked_pid=fork()){
			case -1:
			perror("forked server\n");
			exit(1);
			case 0:
			ProcessData(&sender, &data);
			sendData(&sender, &data);
			printf("Respuesta enviada\n");
			exit(0);
		}
	}
}

void ProcessData(Connection * sender, Datagram * data){

	printf("#FORK: Entro al sleep. 'ATIENDE SERVER...'\n");
	sleep(0);
	printf("FIN DEL SLEEP!\n");
	sender->sender_pid=data->client_pid;
	char* ans;
	int num;
	
	switch(data->opcode){	
		case GET_MOVIE_LIST:
		ans=getMovieList();
		strcpy(data->data.text, ans);
		break;
		case GET_MOVIE_DETAILS:
		ans=getMovieDetails(data->data.i);
		strcpy(data->data.text, ans);
		break;
		case GET_MOVIE_SHOW:
		ans=getMovieShow(data->data.i);
		strcpy(data->data.text, ans);
		break; 
		case GET_SHOW_SEATS:
		ans=getShowSeats(data->data.i);
		strcpy(data->data.text, ans);
		break;
		case BUY_TICKET:
		ans=BuyTicket(data->data.buy.showId, data->data.buy.asiento, data->data.buy.tarjeta,data->data.buy.secCode,data->data.buy.nombre);
		strcpy(data->data.text, ans);
		break;
		case UNDO_BUY_TICKET:
		ans=UndoBuyTicket(data->data.undoBuy.ticketId, data->data.undoBuy.nombre);
		strcpy(data->data.text, ans);		
		break;
		case ADD_SHOW:
		ans=addShow(data->data.addShow.time,data->data.addShow.roomId,data->data.addShow.movieId);
		strcpy(data->data.text, ans);		
		break;
		case REMOVE_SHOW:
		ans=removeShow(data->data.i);
		strcpy(data->data.text, ans);		
		break;
		case ADD_MOVIE:
		ans=addMovie(data->data.movie.length,data->data.movie.title,data->data.movie.desc);
		strcpy(data->data.text, ans);		
		break;
		case REMOVE_MOVIE:
		ans=removeMovie(data->data.i);
		strcpy(data->data.text, ans);		
		break;
		default:
			printf("Comando no soportado!\n");
			sprintf(data->data.text, "COMANDO NO SOPORTADO. OPCODE:%i\n",data->opcode);
	}
	printf("#FORK: esto envia: %s\n", data->data.text);
}
