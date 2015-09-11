#include "communication.h"
#include "datagram.h"
#include "sharedFunctions.h"
#include "sqlite/sqlite3.h"
#include "sqlLib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ProcessData(Connection * sender, Datagram * data);

Datagram data;
Connection sender;


void main(){
	
	setUpDB();
	initChannel(1);
	printf("Server conectado\n");

	int forked_pid;

	printf("Cargado\n");
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
	printf("Entro al sleep. 'ATIENDE SERVER...'\n");
	sleep(2);
	printf("FIN DEL SLEEP!\n");
	sender->sender_pid=data->client_pid;
	char* ans;
	int num;
	switch(data->opcode){	
		case GET_MOVIE_LIST:
		ans=getMovieList();
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
		num=BuyTicket(data->data.buy.showId, data->data.buy.asiento, data->data.buy.tarjeta,data->data.buy.secCode,data->data.buy.nombre);
		data->data.i=num;
		printf("%i\n",num);
		break;
		case UNDO_BUY_TICKET:
		break;
		default:
			printf("Comando no soportado!\n");
			sprintf(data->data.text, "COMANDO NO SOPORTADO. OPCODE:%i\n",data->opcode);
	}
}
