#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "sharedFunctions.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define INPUT_SIZE 		1024
#define COM_SIZE 		12
#define STRING          0
#define INT             1

void cgetMovieList();
void cgetMovieShow(int movieId);
void cgetMovieDetails(int movieId);
void cgetShowSeats(int showId);
void cBuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre);
void cexit();
void cUndoBuyTicket(int ticketId, char* nombre);
char * caddShow(int time, int roomID, int movieID);
char * cremoveShow(int showId);
char * caddMovie(int length, char * title, char * desc);
char * cremoveMovie(int movieID);
void chelp();
void parse(char* buff);
void loadCommands();
int convertArg(char ** args, unsigned char * argTypes, int cant);
int splitArgs(char* args[], char* buffer);

struct command
{
	char * name;
	void (* function) ();
	char * args;
	int argsCant;
	char* desc;
};

static struct command commands[COM_SIZE] = {};


int main (int argc, char const *argv[]) {

	loadCommands();
	connect();
	printf(ANSI_COLOR_GREEN"CLIENTE CONECTADO, abriendo shell..."ANSI_COLOR_RESET"\n");
	char input[INPUT_SIZE];
	int n;
	
	while(1){

		printf(ANSI_COLOR_GREEN":):" ANSI_COLOR_RED );
		fflush(stdout);
		gets(input);
		parse(input);
	}

	printf(ANSI_COLOR_RESET "Cliente termina\n");

	return 0;
}

void loadCommands(){

	commands[0].name = "getMovieList";
	commands[0].function = &cgetMovieList;
	commands[0].argsCant = 0;
	commands[0].desc = "Muestra la cartelera disponible!";

	
	char* getMovieShowArgs = malloc(1000);
	getMovieShowArgs[0]=INT;	
	commands[1].name = "getMovieShow";
	commands[1].function = &cgetMovieShow;
	commands[1].args = getMovieShowArgs;
	commands[1].argsCant = 1;
	commands[1].desc = "Muestra las funciones de una pelicula determinada(movieId).";

	char* getMovieDetailsArgs = malloc(1000);
	getMovieDetailsArgs[0]=INT;	
	commands[2].name = "getMovieDetails";
	commands[2].function = &cgetMovieDetails;
	commands[2].args = getMovieDetailsArgs;
	commands[2].argsCant = 1;
	commands[2].desc = "Muestra los detalles de una pelicula determinada(movieId).";

	char* getShowSeatsArgs = malloc(1000);
	getShowSeatsArgs[0]=INT;
	commands[3].name = "getShowSeats";
	commands[3].function = &cgetShowSeats;
	commands[3].args = getShowSeatsArgs;
	commands[3].argsCant = 1;
	commands[3].desc = "Muestra la disponibilidad de asientos para una funcion determinada (showId).";

	char* BuyTicketArgs = malloc(1000);
	BuyTicketArgs[0]=INT;
	BuyTicketArgs[1]=INT;
	BuyTicketArgs[2]=INT;
	BuyTicketArgs[3]=INT;
	BuyTicketArgs[4]=STRING;
	commands[4].name = "buyTickets";
	commands[4].function = &cBuyTicket;
	commands[4].args = BuyTicketArgs;
	commands[4].argsCant = 5;
	commands[4].desc = "Con showId, asiento, tarjeta, codigo de seguridad y nombre, puedes comprar un ticket.";

	char* UndoBuyTicketArgs= malloc(1000);
	UndoBuyTicketArgs[0]=INT;
	UndoBuyTicketArgs[1]=STRING;
	commands[5].name = "undoBuyTicket";
	commands[5].function = &cUndoBuyTicket;
	commands[5].args = UndoBuyTicketArgs;
	commands[5].argsCant = 2;
	commands[5].desc = "Deshace la compra recibiendo ticketId y nombre del comprador.";

	char* addShowArgs= malloc(1000);
	addShowArgs[0]=INT;
	addShowArgs[1]=INT;
	addShowArgs[2]=INT;
	commands[6].name = "addShow";
	commands[6].function = &caddShow;
	commands[6].args = addShowArgs;
	commands[6].argsCant = 3;
	commands[6].desc = "Agrega una funcion dada una hora, sala y pelicula.";

	char* removeShowArgs= malloc(1000);
	removeShowArgs[0]=INT;
	commands[7].name = "removeShow";
	commands[7].function = &cremoveShow;
	commands[7].args = removeShowArgs;
	commands[7].argsCant = 1;
	commands[7].desc = "Remueve una funcion medienta su id.";

	char* addMovieArgs= malloc(1000);
	addMovieArgs[0]=INT;
	addMovieArgs[1]=STRING;
	addMovieArgs[2]=STRING;
	commands[8].name = "addMovie";
	commands[8].function = &caddMovie;
	commands[8].args = addMovieArgs;
	commands[8].argsCant = 3;
	commands[8].desc = "Agrega una pelicula con su titulo, descripcion y duracion.";

	char* removeMovieArgs= malloc(1000);
	removeMovieArgs[0]=INT;
	commands[9].name = "removeMovie";
	commands[9].function = &cremoveMovie;
	commands[9].args = removeMovieArgs;
	commands[9].argsCant = 1;
	commands[9].desc = "Remueve una pelicula medienta su id.";

	commands[10].name = "exit";
	commands[10].function = &cexit;
	commands[10].argsCant = 0;
	commands[10].desc = "Sale del programa.";

	commands[11].name = "help";
	commands[11].function = &chelp;
	commands[11].argsCant = 0;
	commands[11].desc = "I need somebody... ";

}

void parse(char* buff){
	int index, cant;
	char flag=0;
	char* args[6];

	cant=splitArgs(args,buff);
	// The first input is not a parameter
	cant-=1;

	for(index = 0;!flag &&  index<COM_SIZE ; index++){
		if(!strcmp(args[0], commands[index].name)){
			flag=1;

			if(cant!=commands[index].argsCant){
				printf("Se esperaban %i argumentos y se recibieron %i, revisa el comando \n",commands[index].argsCant,cant);
			}else{
				convertArg(args, commands[index].args, commands[index].argsCant);
				switch(commands[index].argsCant){
					case 0:
					commands[index].function();
					break;
					case 1:
					commands[index].function(args[1]);
					break;
					case 2:
					commands[index].function(args[1], args[2]);
					break;
					case 3:
					commands[index].function(args[1], args[2], args[3]);
					break;
					case 5:
					commands[index].function(args[1], args[2], args[3], args[4], args[5]);
					break;
				}
			}
		}
	}
	if(!flag){
		printf(ANSI_COLOR_MAGENTA "Comando invalido: %s. Intente el comando 'help'\n",args[0]);
	}
}

int splitArgs(char* args[], char* buffer){
	int cont=1;
	for(args[0]=buffer;*buffer != '\0'; buffer++){
		if(*buffer==' '){
			*buffer='\0';
			buffer++;
			args[cont++]=buffer;
		}
	}
	//*buffer='\0';
	return cont;
}

int convertArg(char ** args, unsigned  char * argTypes, int cant){
	int  j;
	for(j=1; j<=cant; j++){	
		switch(argTypes[j-1]){
			case INT:
			args[j]=atoi(args[j]);
			break;
			default:
			break;
		}
	}
}

void chelp(){
	int i,j;
	for(i=0;i<COM_SIZE;i++){
		printf(ANSI_COLOR_GREEN "------ %s -------\n", commands[i].name);
		if(commands[i].argsCant!=0){
			printf(ANSI_COLOR_MAGENTA"Argumentos: ");
			for(j=0;j<commands[i].argsCant;j++){
				printf(ANSI_COLOR_CYAN "%s ",(commands[i].args[j]==INT)?"INT":"STRING");
			}
		}else{
			printf(ANSI_COLOR_MAGENTA"No tiene argumentos ");
		}
		printf(ANSI_COLOR_MAGENTA"\nDescripcion:"  ANSI_COLOR_CYAN " %s\n",commands[i].desc);

	}
}

void cgetMovieList(){
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO CARTELERA ----\n");
	char* answer=getMovieList();
	char* aux1;
	char* aux2;
	int cont=0;
	int flag=0;
	while(!flag){
		if(cont==0){
			aux1=answer;
			cont=1;
		}else{
			while(answer[0]!=';' && answer[0]!='\0'){
				answer++;
			}
			if(cont==1){
				if(answer[0]==';'){
					answer[0]='\0';
					answer++;
					aux2=answer;
					cont=2;
				}else{
					printf("ERROR?\n");
					flag=1;
				}
			}else{
				answer[0]='\0';
				answer++;
				printf(ANSI_COLOR_MAGENTA"Nombre:"ANSI_COLOR_CYAN " %s " ANSI_COLOR_MAGENTA " Id de la pelicula:"ANSI_COLOR_CYAN" %s\n",aux1,aux2);
				cont=0;
				if(answer[0]=='\0')
					flag=1;
			}
		}
	}
}

void
cgetMovieShow(int movieId){
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO FUNCIONES ----\n");
	char* answer=getMovieShow(movieId);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);
}

void
cgetMovieDetails(int movieId){
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO FUNCIONES ----\n");
	char* answer=getMovieDetails(movieId);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);
}

void
cgetShowSeats(int showId){
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO ASIENTOS ----\n");
	char* answer=getShowSeats(showId);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);
}

void cBuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){
	printf(ANSI_COLOR_CYAN"---- REALIZANDO COMPRA ----\n");
	char* answer=BuyTicket(showId,  asiento,  tarjeta, secCode, nombre);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);
}

void cUndoBuyTicket(int ticketId, char* nombre){
	printf(ANSI_COLOR_CYAN"---- DESHACIENDO COMPRA ----\n");
	char* answer=UndoBuyTicket(ticketId, nombre);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);	
}

char * caddShow(int time, int roomID, int movieID){
	printf(ANSI_COLOR_CYAN"---- AGREGANDO SHOW ----\n");
	char* answer=addShow(time,roomID,movieID);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);	
}

char * cremoveShow(int showId){
	printf(ANSI_COLOR_CYAN"---- REMOVIENDO SHOW ----\n");
	char* answer=removeShow(showId);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);	
}
char * caddMovie(int length, char * title, char * desc){
	printf(ANSI_COLOR_CYAN"---- AGREGANDO PELICULA ----\n");
	char* answer=addMovie(length,title,desc);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);	
}

char * cremoveMovie(int movieID){
	printf(ANSI_COLOR_CYAN"---- REMOVIENDO PELICULA ----\n");
	char* answer=removeMovie(movieID);
	printf(ANSI_COLOR_MAGENTA" %s ",answer);	
}

void cexit(){
	printf(ANSI_COLOR_BLUE"Saliendo!" ANSI_COLOR_RESET "\n");
	exit(0);
}
