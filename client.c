//#include "sharedFunctions.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "sharedFunctions.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define INPUT_SIZE 1024
void parse(char* buff);
int main (int argc, char const *argv[]) {

/*
printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");
printf(ANSI_COLOR_GREEN   "This text is GREEN!"   ANSI_COLOR_RESET "\n");
printf(ANSI_COLOR_YELLOW  "This text is YELLOW!"  ANSI_COLOR_RESET "\n");
printf(ANSI_COLOR_BLUE    "This text is BLUE!"    ANSI_COLOR_RESET "\n");
printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!" ANSI_COLOR_RESET "\n");
printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");
*/

	connect();
	printf(ANSI_COLOR_GREEN"CLIENTE CONECTADO, abriendo shell..."ANSI_COLOR_RESET"\n");

	char* input=malloc(INPUT_SIZE);
	int n;
	
	while(1){
		printf(ANSI_COLOR_GREEN":) " ANSI_COLOR_CYAN  " :" ANSI_COLOR_RED );
		fflush(stdout);

		fgets(input,INPUT_SIZE, stdin);
		char* aux=input;
		while(*(aux++));
		aux-=2;
		*(aux)=0;


		parse(input);
		
	}

	printf(ANSI_COLOR_RESET "Cliente termina\n");

	return 0;
}

const char * accepted_commands[] = {
    "exit",
    "getMovieList",
};
int accepted_commands_size=2;

void parse(char* buff){

	int i;
	for(i=0;i<accepted_commands_size;i++){
		if(!strcmp(accepted_commands[i],buff)){
			if(i==0){
				printf("Chau =D"ANSI_COLOR_RESET" \n");
				exit(0);
			}else if(i==1){
				getMovieList();
			}
		}
	}
}