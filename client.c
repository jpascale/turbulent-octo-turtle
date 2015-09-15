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
#define COM_SIZE 		11
#define STRING          0
#define INT             1

void cgetMovieList();
void cgetMovieShow(int i);
void cgetShowSeats(int i);
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

typedef void (* func) ();

struct command
{
	char * name;
	func function;
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
	commands[0].function = (func)&cgetMovieList;
	commands[0].argsCant = 0;
	commands[0].desc = "Muestra la cartelera disponible!";

	
	char* getMovieShowArgs = malloc(1000);
	getMovieShowArgs[0]=INT;	
	commands[1].name = "getMovieShow";
	commands[1].function = (func)&cgetMovieShow;
	commands[1].args = getMovieShowArgs;
	commands[1].argsCant = 1;
	commands[1].desc = "Muestra las funciones de una pelicula determinada(movieId).";

	char* getShowSeatsArgs = malloc(1000);
	getShowSeatsArgs[0]=INT;
	commands[2].name = "getShowSeats";
	commands[2].function = (func)&cgetShowSeats;
	commands[2].args = getShowSeatsArgs;
	commands[2].argsCant = 1;
	commands[2].desc = "Muestra la disponibilidad de asientos para una funcion determinada (showId).";

	char* BuyTicketArgs = malloc(1000);
	BuyTicketArgs[0]=INT;
	BuyTicketArgs[1]=INT;
	BuyTicketArgs[2]=INT;
	BuyTicketArgs[3]=INT;
	BuyTicketArgs[4]=STRING;
	commands[3].name = "buyTickets";
	commands[3].function = (func)&cBuyTicket;
	commands[3].args = BuyTicketArgs;
	commands[3].argsCant = 5;
	commands[3].desc = "Con showId, asiento, tarjeta, codigo de seguridad y nombre, puedes comprar un ticket.";

	char* UndoBuyTicketArgs= malloc(1000);
	UndoBuyTicketArgs[0]=INT;
	UndoBuyTicketArgs[1]=STRING;
	commands[4].name = "undoBuyTicket";
	commands[4].function = (func)&cUndoBuyTicket;
	commands[4].args = UndoBuyTicketArgs;
	commands[4].argsCant = 2;
	commands[4].desc = "Deshace la compra recibiendo ticketId y nombre del comprador.";

	char* addShowArgs= malloc(1000);
	addShowArgs[0]=INT;
	addShowArgs[1]=INT;
	addShowArgs[2]=INT;
	commands[5].name = "addShow";
	commands[5].function = (func)&caddShow;
	commands[5].args = addShowArgs;
	commands[5].argsCant = 3;
	commands[5].desc = "Agrega una funcion dada una hora, sala y pelicula.";

	char* removeShowArgs= malloc(1000);
	removeShowArgs[0]=INT;
	commands[6].name = "removeShow";
	commands[6].function = (func)&cremoveShow;
	commands[6].args = removeShowArgs;
	commands[6].argsCant = 1;
	commands[6].desc = "Remueve una funcion medienta su id.";


	char* addMovieArgs= malloc(1000);
	addMovieArgs[0]=INT;
	addMovieArgs[1]=STRING;
	addMovieArgs[2]=STRING;
	commands[7].name = "addMovie";
	commands[7].function = (func)&caddMovie;
	commands[7].args = addMovieArgs;
	commands[7].argsCant = 3;
	commands[7].desc = "Agrega una pelicula con su titulo, descripcion y duracion.";

	char* removeMovieArgs= malloc(1000);
	removeMovieArgs[0]=INT;
	commands[8].name = "removeMovie";
	commands[8].function = (func)&cremoveMovie;
	commands[8].args = removeMovieArgs;
	commands[8].argsCant = 1;
	commands[8].desc = "Remueve una pelicula medienta su id.";

	commands[9].name = "exit";
	commands[9].function = (func)&cexit;
	commands[9].argsCant = 0;
	commands[9].desc = "Sale del programa.";

	commands[10].name = "help";
	commands[10].function = (func)&chelp;
	commands[10].argsCant = 0;
	commands[10].desc = "I need somebody... ";

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
cgetMovieShow(int i){
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO FUNCIONES ----\n");
	char* answer=getMovieShow(i);
	printf(ANSI_COLOR_GREEN" %s ",answer);
}

void
cgetShowSeats(int i){
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO ASIENTOS ----\n");
	char* answer=getShowSeats(i);
	printf(ANSI_COLOR_GREEN" %s ",answer);
}

void cBuyTicket(int showId, int asiento, int tarjeta,int secCode, char* nombre){
	printf(ANSI_COLOR_CYAN"---- REALIZANDO COMPRA ----\n");
	char* answer=BuyTicket(showId,  asiento,  tarjeta, secCode, nombre);
	printf(ANSI_COLOR_GREEN" %s ",answer);
}

void cUndoBuyTicket(int ticketId, char* nombre){
	printf(ANSI_COLOR_CYAN"---- DESHACIENDO COMPRA ----\n");
	char* answer=UndoBuyTicket(ticketId, nombre);
	printf(ANSI_COLOR_GREEN" %s ",answer);	
}

char * caddShow(int time, int roomID, int movieID){
	printf(ANSI_COLOR_CYAN"---- AGREGANDO SHOW ----\n");
	char* answer=addShow(time,roomID,movieID);
	printf(ANSI_COLOR_GREEN" %s ",answer);	
}

char * cremoveShow(int showId){
	printf(ANSI_COLOR_CYAN"---- REMOVIENDO SHOW ----\n");
	char* answer=removeShow(showId);
	printf(ANSI_COLOR_GREEN" %s ",answer);	
}
char * caddMovie(int length, char * title, char * desc){
	printf(ANSI_COLOR_CYAN"---- AGREGANDO PELICULA ----\n");
	char* answer=addMovie(length,title,desc);
	printf(ANSI_COLOR_GREEN" %s ",answer);	
}

char * cremoveMovie(int movieID){
	printf(ANSI_COLOR_CYAN"---- REMOVIENDO PELICULA ----\n");
	char* answer=removeMovie(movieID);
	printf(ANSI_COLOR_GREEN" %s ",answer);	
}

void cexit(){
	printf(ANSI_COLOR_BLUE"Saliendo!" ANSI_COLOR_RESET "\n");
	exit(0);
}
