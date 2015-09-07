#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "sharedFunctions.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define INPUT_SIZE 		1024
#define COM_SIZE 		9
#define STRING          0
#define INT             1


int convertArg(char ** args, unsigned char * argTypes, int cant);
void parse(char* buff);
void loadCommands();
void cexit();
void help();

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
    commands[0].function = &getMovieList;
    commands[0].argsCant = 0;
    commands[0].desc = "Muestra la cartelera disponible!";

  
	char* getMovieShowArgs = malloc(1000);
    getMovieShowArgs[0]=INT;	
    commands[1].name = "getMovieShow";
    commands[1].function = &getMovieShow;
    commands[1].args = getMovieShowArgs;
    commands[1].argsCant = 1;
    commands[1].desc = "Muestra las funciones de una pelicula determinada(movieId).";

	char* getShowSeatsArgs = malloc(1000);
    getShowSeatsArgs[0]=INT;
    commands[2].name = "getShowSeats";
    commands[2].function = &getShowSeats;
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
    commands[3].function = &BuyTicket;
    commands[3].args = BuyTicketArgs;
    commands[3].argsCant = 5;
    commands[3].desc = "Con showId, asiento, tarjeta, codigo de seguridad y nombre, puedes comprar un ticket.";

	char* UndoBuyTicketArgs= malloc(1000);
	UndoBuyTicketArgs[0]=INT;
	UndoBuyTicketArgs[1]=STRING;
    commands[4].name = "undoBuyTicket";
    commands[4].function = &UndoBuyTicket;
    commands[4].args = UndoBuyTicketArgs;
    commands[4].argsCant = 2;
    commands[4].desc = "Deshace la compra recibiendo ticketId y nombre del comprador.";

	char* addFunctionArgs= malloc(1000);
	addFunctionArgs[0]=STRING;
    commands[5].name = "addFunction";
    commands[5].function = &addFunction;
    commands[5].args = addFunctionArgs;
    commands[5].argsCant = 1;
    commands[5].desc = "Agrega una pelicula a la cartelera.";

	char* removeFunctionArgs= malloc(1000);
	removeFunctionArgs[0]=STRING;
    commands[6].name = "removeFunction";
    commands[6].function = &removeFunction;
    commands[6].args = removeFunctionArgs;
    commands[6].argsCant = 1;
    commands[6].desc = "Remueve una pelicula a la cartelera.";

    commands[7].name = "exit";
    commands[7].function = &cexit;
    commands[7].argsCant = 0;
    commands[7].desc = "Sale del programa.";

    commands[8].name = "help";
    commands[8].function = &help;
    commands[8].argsCant = 0;
    commands[8].desc = "I need somebody... ";
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
				case 5:
					commands[index].function(args[1], args[2], args[3], args[4], args[5]);
					break;
					}
				}
			}
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
	*buffer='\0';
	return cont;
}

int convertArg(char ** args, unsigned  char * argTypes, int cant){
	int  j;
	for(j=1; j<=cant; j++){
		printf("Ciclo con %i\n",argTypes[j-1]);
		
		switch(argTypes[j-1]){
			case INT:
				args[j]=atoi(args[j]);
				break;
			default:
				break;
		}	
	}
	
}

void help(){
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

void cexit(){
	printf("Saliendo!" ANSI_COLOR_RESET "\n");
	exit(0);

}
