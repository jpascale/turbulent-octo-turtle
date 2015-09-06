//#include "sharedFunctions.h"
#include <stdio.h>
#include <fcntl.h>

#include "sharedFunctions.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define INPUT_SIZE 1024
#define COM_SIZE 2

int convertArg(char ** args, char * argTypes, int cant);
void parse(char* buff);
void loadCommands();
void cexit();

struct command
{
        char * name;
        void (* function) ();
        unsigned char * args;
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
		printf("Logged\n");
		parse(input);
		
	}

	printf(ANSI_COLOR_RESET "Cliente termina\n");

	return 0;
}

void loadCommands(){
	unsigned char clearArgs[] = {};
    commands[0].name = "getMovieList";
    commands[0].function = &getMovieList;
    commands[0].args = clearArgs;
    commands[0].argsCant = 0;
    commands[0].desc = "Clears the screen, restoring it to its purest state";

	unsigned char echoArgs[] = {};
    commands[1].name = "exit";
    commands[1].function = &cexit;
    commands[1].args = echoArgs;
    commands[1].argsCant = 2;
    commands[1].desc = "Prints a given string with the provided format.";
}

void parse(char* buff){
int index;
for(index = 0; index<COM_SIZE ; index++){
if(!strcmp(buff, commands[index].name)){
//	convertArg(args, commands[index].args, commands[index].argsCant);
	
	switch(commands[index].argsCant){
	case 0:
		commands[index].function();
		break;
	case 1:
//		commands[index].function(args[1]);
		break;
	case 2:
//		commands[index].function(args[1], args[2]);
		break;
	case 3:
//		commands[index].function(args[1], args[2], args[3]);
		break;
		}
		}
	}
}

void cexit(){
	printf("Saliendo!" ANSI_COLOR_RESET "\n");
	exit(0);
}
