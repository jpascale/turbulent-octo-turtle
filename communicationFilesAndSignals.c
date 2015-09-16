#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#include "./datagram.h"

int leftStringMatch(char * begin, char * string);
void mypause(int sign);

int is_server, reading = 0, fd, aux;
DIR *dir;
struct dirent *ent;	
sigset_t mask, oldmask;
char auxS[32];

initChannel(int bool_server){
	is_server = bool_server;
	if(is_server){
		signal(SIGUSR1, mypause);
		fd = open("/tmp/server_pid", O_CREAT | O_WRONLY);
		aux = getpid();
		printf("mi pid es: %i\n",  aux);
		write(fd, &aux, sizeof(int));
		dir = opendir ("/tmp/");
	}
}

sendData(Connection * connection, Datagram * params){
	fd = open("/tmp/server_pid", O_RDONLY);
	read(fd, auxS, sizeof(int));
	aux = *((int*)auxS);
	printf("server pid es: %i\n",  aux);
	kill(aux, SIGUSR1);
	
}

receiveData(Connection * sender, Datagram * buffer){

	sigemptyset (&mask);
	sigaddset (&mask, SIGUSR1);
	
	sigprocmask (SIG_BLOCK, &mask, &oldmask);
	sleep(20);
	printf("termino el sleep\n");
	sigsuspend(&oldmask);
	printf("libre!\n");
	while(1);
	/*if(is_server){
		sigprocmask();
		if(reading){
			while ((ent = readdir (dir)) != NULL && !leftStringMatch("request", ent->d_name));
			if(ent != NULL){
				sprintf(buffer, "/tmp/%s", ent->d_name);
				fd = open(buffer, O_RDONLY);
				read(fd, buffer, sizeof(int));
				//printf("esto leyo: %i\n", (int*)buffer);
				int size = *((int*)buffer);
				read(fd, ((char*)buffer)+sizeof(int), size - sizeof(int));
				return;
			}else{
				sigsuspend();
			}
  			//closedir (dir);
		}else{
			sigsuspend();
		}
	}*/
}
leftStringMatch(char * begin, char * string){
	int match = 1;
	while (*begin!=0 && match) if(*(begin++)!=*(string++)) match = 0;
	return match;
}

void mypause(int sign){
	printf("entro a la señal\n");
}