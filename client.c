#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "sharedFunctions.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define INPUT_SIZE 		1024
#define COM_SIZE 		13
#define STRING          0
#define INT             1

#define NEW_LINE 		13
#define BACKSPACE		127
#define TAB				'\t'


void cgetMovieList();
void cgetMovieShow(int movieId);
void cgetMovieDetails(int movieId);
void cgetShowSeats(int showId);
void cBuyTicket(int showId, int asiento, int tarjeta, int secCode, char* nombre);
void cexit();
void cUndoBuyTicket(int ticketId, char* nombre);
void caddShow(int time, int roomID, int movieID);
void cremoveShow(int showId);
void caddMovie(int length, char * title, char * desc);
void cremoveMovie(int movieID);
void chelp();
void parse(char* buff);
void loadCommands();
int convertArg(char ** args, unsigned char * argTypes, int cant);
int splitArgs(char* args[], char* buffer);
typedef void (* func) ();
//void getLine(char * buffer);
//int autoComplete(char* buffer, int i, char* completed);
void csignal(int sig);
void cclear();

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
	//::::::

	//::::::
	signal(SIGINT, csignal);
	loadCommands();
	__connect();
	printf(ANSI_COLOR_GREEN"CLIENTE CONECTADO, abriendo shell..."ANSI_COLOR_RESET"\n");
	char input[INPUT_SIZE];
	int n;

	while (1) {

		printf(ANSI_COLOR_GREEN":):" ANSI_COLOR_RED );
		fflush(stdout);
		//	getLine(input);
		fgets(input, INPUT_SIZE - 16, stdin);
		int len = strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
		parse(input);
	}

	printf(ANSI_COLOR_RESET "Cliente termina\n");

	return 0;
}

/*
// Tomo el control del input para poder dar más funcionalidades.
// Si bien le saca portabilidad, en el contexto de evaluacion
// no va a traer ningun efecto inesperado
void getLine(char * buffer) {
// Hace que el input de stdin se mande crudo, sin necesidad de enter
	char c;
	char completed[100];
	completed[0] = 0;
	int i = 0;
	char* iter = buffer;
	system ("/bin/stty raw -echo isig");
	while ((c = getchar()) != NEW_LINE) {
		if (c == BACKSPACE) {
			printf("\b  \b\b");
			i--;
			iter[i] = 0;
		} else if (c == TAB) {
			if (autoComplete(buffer, i, completed) == 1) {
				int m;
				for (m = 0; m < i; m++)
					fprintf(stdout, "\b");
				printf("%s", completed);

				for (m = 0; completed[m] != 0; m++) {
					buffer[m] = completed[m];
				}
				i = m;
			}
		} else if (c == '\033') {
			//Ingreso una flecha. Por el momento no estan soportadas
			// if the first value is esc
			getchar(); // skip the [
			switch (getchar()) { // the real value
			case 'A':
				// code for arrow up
				break;
			case 'B':
				// code for arrow down
				break;
			case 'C':
				// code for arrow right
				break;
			case 'D':
				// code for arrow left
				break;
			}
		} else {
			putc(c, stdout);
			iter[i] = c;
			i++;
		}
	}
	iter[i] = 0;
// Se restablece el default. Se asume que era el previo al llamado.
	system ("/bin/stty cooked echo");
// Remuevo la marca de ENTER en la pantalla. Asumo \b caracter no destructivo
	printf("\n");
}

//ret 0 if not match or multiple matches
int autoComplete(char* buffer, int i, char* completed) {
	char matches = 0, flag;
	char* matched;
	int aux, c;
	char* currentCom;
	for (c = 0; c < COM_SIZE && matches < 2; c++) {
		currentCom = commands[c].name;
		for (aux = 0, flag = 0; !flag && currentCom[aux] != 0 && aux < i; aux++) {
			if (currentCom[aux] != buffer[aux]) {
				flag = 1;
			}
		}
		if (!flag && aux == i && currentCom[aux] != 0) {
			matches++;
			matched = commands[c].name;
		}
	}
	if (matches == 1) {
		strcpy(completed, matched);
	}
	return matches;
}
*/

void loadCommands() {

	commands[0].name = "getMovieList";
	commands[0].function = (func)&cgetMovieList;
	commands[0].argsCant = 0;
	commands[0].desc = "Muestra la cartelera disponible!";


	char* getMovieShowArgs = malloc(1000);
	getMovieShowArgs[0] = INT;
	commands[1].name = "getMovieShow";
	commands[1].function = (func)&cgetMovieShow;
	commands[1].args = getMovieShowArgs;
	commands[1].argsCant = 1;
	commands[1].desc = "Muestra las funciones de una pelicula determinada(movieId).";

	char* getMovieDetailsArgs = malloc(1000);
	getMovieDetailsArgs[0] = INT;
	commands[2].name = "getMovieDetails";
	commands[2].function = (func)&cgetMovieDetails;
	commands[2].args = getMovieDetailsArgs;
	commands[2].argsCant = 1;
	commands[2].desc = "Muestra los detalles de una pelicula determinada(movieId).";

	char* getShowSeatsArgs = malloc(1000);
	getShowSeatsArgs[0] = INT;

	commands[3].name = "getShowSeats";
	commands[3].function = (func)&cgetShowSeats;
	commands[3].args = getShowSeatsArgs;
	commands[3].argsCant = 1;
	commands[3].desc = "Muestra la disponibilidad de asientos para una funcion determinada (showId).";

	char* BuyTicketArgs = malloc(1000);
	BuyTicketArgs[0] = INT;
	BuyTicketArgs[1] = INT;
	BuyTicketArgs[2] = INT;
	BuyTicketArgs[3] = INT;
	BuyTicketArgs[4] = STRING;

	commands[4].name = "buyTickets";
	commands[4].function = (func)&cBuyTicket;
	commands[4].args = BuyTicketArgs;
	commands[4].argsCant = 5;
	commands[4].desc = "Con showId, asiento, tarjeta, codigo de seguridad y nombre, puedes comprar un ticket.";


	char* UndoBuyTicketArgs = malloc(1000);
	UndoBuyTicketArgs[0] = INT;
	UndoBuyTicketArgs[1] = STRING;

	commands[5].name = "undoBuyTicket";
	commands[5].function = (func)&cUndoBuyTicket;
	commands[5].args = UndoBuyTicketArgs;
	commands[5].argsCant = 2;
	commands[5].desc = "Deshace la compra recibiendo ticketId y nombre del comprador.";

	char* addShowArgs = malloc(1000);
	addShowArgs[0] = INT;
	addShowArgs[1] = INT;
	addShowArgs[2] = INT;

	commands[6].name = "addShow";
	commands[6].function = (func)&caddShow;
	commands[6].args = addShowArgs;
	commands[6].argsCant = 3;
	commands[6].desc = "Agrega una funcion dada una hora, sala y pelicula.";

	char* removeShowArgs = malloc(1000);
	removeShowArgs[0] = INT;
	commands[7].name = "removeShow";
	commands[7].function = (func)&cremoveShow;
	commands[7].args = removeShowArgs;
	commands[7].argsCant = 1;
	commands[7].desc = "Remueve una funcion medienta su id.";

	char* addMovieArgs = malloc(1000);
	addMovieArgs[0] = INT;
	addMovieArgs[1] = STRING;
	addMovieArgs[2] = STRING;

	commands[8].name = "addMovie";
	commands[8].function = (func)&caddMovie;
	commands[8].args = addMovieArgs;
	commands[8].argsCant = 3;
	commands[8].desc = "Agrega una pelicula con su titulo, descripcion y duracion.";

	char* removeMovieArgs = malloc(1000);
	removeMovieArgs[0] = INT;
	commands[9].name = "removeMovie";
	commands[9].function = (func)&cremoveMovie;
	commands[9].args = removeMovieArgs;
	commands[9].argsCant = 1;
	commands[9].desc = "Remueve una pelicula medienta su id.";

	commands[10].name = "exit";
	commands[10].function = (func)&cexit;

	commands[10].argsCant = 0;
	commands[10].desc = "Sale del programa.";

	commands[11].name = "help";
	commands[11].function = (func)&chelp;
	commands[11].argsCant = 0;
	commands[11].desc = "I need somebody... ";

	commands[12].name = "clear";
	commands[12].function = (func)&cclear;
	commands[12].argsCant = 0;
	commands[12].desc = "No explanation. It's pretty clear =D";

}

void parse(char* buff) {
	int index, cant;
	char flag = 0;
	char* args[6];

	cant = splitArgs(args, buff);

	if (cant == -1) {
		printf(ANSI_COLOR_RED "No injection allowed\n" ANSI_COLOR_RESET);
		return;
	}

	// The first input is not a parameter
	cant -= 1;

	for (index = 0; !flag &&  index < COM_SIZE ; index++) {
		if (!strcmp(args[0], commands[index].name)) {
			flag = 1;

			if (cant != commands[index].argsCant) {
				printf("Se esperaban %i argumentos y se recibieron %i, revisa el comando \n", commands[index].argsCant, cant);
			} else {
				convertArg(args, commands[index].args, commands[index].argsCant);
				switch (commands[index].argsCant) {
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
	if (!flag) {
		printf(ANSI_COLOR_MAGENTA "Comando invalido: %s. Intente el comando 'help'\n", args[0]);
	}
}

int splitArgs(char* args[], char* buffer) {
	int i = 0, m = 0, j = 1, flag = 0, error = 0;
	args[0] = buffer;
	while (buffer[i] && !error) {
		if (buffer[i] == '\'') {
			error = 1;
		} else if (buffer[i] == ' ' && !flag) {
			buffer[i] = 0;
			args[j++] = buffer + i + 1;
		} else if (buffer[i] == '\"') {
			if (!flag)
				args[j - 1] = buffer + i + 1;
			else
				buffer[i] = 0;
			flag = 1 - flag;
		}
		i++;
	}
	if (error) {
		return -1;
	}
	return j;
}

int convertArg(char ** args, unsigned  char * argTypes, int cant) {
	int  j;
	for (j = 1; j <= cant; j++) {
		switch (argTypes[j - 1]) {
		case INT:
			args[j] = (char *)atoi(args[j]);
			break;
		default:
			break;
		}
	}
}

void chelp() {
	int i, j;
	for (i = 0; i < COM_SIZE; i++) {
		printf(ANSI_COLOR_GREEN "------ %s -------\n", commands[i].name);
		if (commands[i].argsCant != 0) {
			printf(ANSI_COLOR_MAGENTA"Argumentos: ");
			for (j = 0; j < commands[i].argsCant; j++) {
				printf(ANSI_COLOR_CYAN "%s ", (commands[i].args[j] == INT) ? "INT" : "STRING");
			}
		} else {
			printf(ANSI_COLOR_MAGENTA"No tiene argumentos ");
		}
		printf(ANSI_COLOR_MAGENTA"\nDescripcion:"  ANSI_COLOR_CYAN " %s\n", commands[i].desc);

	}
}

void cgetMovieList() {
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO CARTELERA ----\n");
	char* answer = getMovieList();
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}

	char* aux1;
	char* aux2;
	int cont = 0;
	int flag = 0;
	while (!flag) {
		if (cont == 0) {
			aux1 = answer;
			cont = 1;
		} else {
			while (answer[0] != ';' && answer[0] != '\0') {
				answer++;
			}
			if (cont == 1) {
				if (answer[0] == ';') {
					answer[0] = '\0';
					answer++;
					aux2 = answer;
					cont = 2;
				} else {
					printf("Fin de entrada inesperado\n");
					flag = 1;
				}
			} else {
				answer[0] = '\0';
				answer++;
				printf(ANSI_COLOR_MAGENTA"Nombre:"ANSI_COLOR_CYAN " %s " ANSI_COLOR_MAGENTA " Id de la pelicula:"ANSI_COLOR_CYAN" %s\n", aux1, aux2);
				cont = 0;
				if (answer[0] == '\0')
					flag = 1;
			}
		}
	}

}

void
cgetMovieShow(int movieId) {
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO FUNCIONES ----\n");
	char* answer = getMovieShow(movieId);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}

void
cgetMovieDetails(int movieId) {
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO FUNCIONES ----\n");
	char* answer = getMovieDetails(movieId);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}

void
cgetShowSeats(int showId) {
	printf(ANSI_COLOR_CYAN"---- CONSULTANDO ASIENTOS ----\n");
	char* answer = getShowSeats(showId);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA"Asientos ocupados  en el show" ANSI_COLOR_GREEN" %i:\n", showId);
	printf(ANSI_COLOR_RED "%s\n", answer);
}

void cBuyTicket(int showId, int asiento, int tarjeta, int secCode, char* nombre) {
	printf(ANSI_COLOR_CYAN "---- REALIZANDO COMPRA ----\n");
	char* answer = BuyTicket(showId,  asiento,  tarjeta, secCode, nombre);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA " %s ", answer);
}

void cUndoBuyTicket(int ticketId, char* nombre) {
	printf(ANSI_COLOR_CYAN"---- DESHACIENDO COMPRA ----\n");
	char* answer = UndoBuyTicket(ticketId, nombre);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}

void caddShow(int time, int roomID, int movieID) {
	printf(ANSI_COLOR_CYAN"---- AGREGANDO SHOW ----\n");
	char* answer = addShow(time, roomID, movieID);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}

void cremoveShow(int showId) {
	printf(ANSI_COLOR_CYAN"---- REMOVIENDO SHOW ----\n");
	char* answer = removeShow(showId);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}
void caddMovie(int length, char * title, char * desc) {
	printf(ANSI_COLOR_CYAN"---- AGREGANDO PELICULA ----\n");
	char* answer = addMovie(length, title, desc);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}

void cremoveMovie(int movieID) {
	printf(ANSI_COLOR_CYAN"---- REMOVIENDO PELICULA ----\n");
	char* answer = removeMovie(movieID);
	if (answer[0] == 0) {
		printf(ANSI_COLOR_RED"Server not found\n"ANSI_COLOR_RESET);
		return;
	}
	printf(ANSI_COLOR_MAGENTA" %s ", answer);
}

void cexit() {
	printf(ANSI_COLOR_BLUE"Saliendo!" ANSI_COLOR_RESET "\n");
	exit(0);
}

void cclear() {
	printf("\e[1;1H\e[2J");
}

void csignal(int sig) {
	printf(ANSI_COLOR_RESET);
	handOff(sig);
	exit(1);
}
