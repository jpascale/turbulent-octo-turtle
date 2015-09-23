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

int is_server, reading = 0, fd, server_pid, aux = 1;
DIR *dir;
struct dirent *ent;
sigset_t mask, oldmask;
char auxS[32], writeFileName[32], readFileName[32];

initChannel(int bool_server) {
	is_server = bool_server;
	signal(SIGUSR1, mypause);
	signal(SIGUSR2, mypause);
	if (is_server) { //server
		remove("/tmp/server_pid");
		fd = open("/tmp/server_pid", O_CREAT | O_WRONLY, 777);
		server_pid = getpid();
		write(fd, &server_pid, sizeof(int));
		close(fd);
		fd = open("/tmp/server_pid", O_RDONLY);
		read(fd, auxS, sizeof(int));
		server_pid = *((int*)auxS);
		printf("server pid es: %i\n",  server_pid);
		close(fd);
		reading = 0;
	} else { //client
		fd = open("/tmp/server_pid", O_RDONLY | O_CREAT, 777);
		read(fd, auxS, sizeof(int));
		close(fd);
		server_pid = *((int*)auxS);
		printf("Mi pid es: %i\n",  getpid());
		sprintf(writeFileName, "/tmp/request_%d", getpid());
		sprintf(readFileName, "/tmp/response_%d", getpid());
	}
}

sendData(Connection * connection, Datagram * params) {

	if (is_server) {
		sprintf(writeFileName, "/tmp/response_%i", connection->sender_pid);
		fd = open(writeFileName, O_CREAT | O_WRONLY, 777);
		write(fd, params, *(int*)params);
		kill(connection->sender_pid, SIGUSR1);
		close(fd);
	} else {
		fd = open(writeFileName, O_CREAT | O_WRONLY, 777);
		write(fd, params, *(int*)params);
		kill(server_pid, SIGUSR1);
		close(fd);
	}

}

receiveData(Connection * sender, Datagram * buffer) {
	if (is_server) { //server
		while (1) {
			if (!reading) {
				if (aux) {
					sigemptyset (&mask);
					sigaddset (&mask, SIGUSR1);
					sigprocmask (SIG_BLOCK, &mask, &oldmask);
					aux = 0;
				}

				sigsuspend(&oldmask);

				reading = 1;
				dir = opendir ("/tmp/");
			}
			while ((ent = readdir (dir)) != NULL && !leftStringMatch("request", ent->d_name));
			if (ent != NULL) {
				sprintf(readFileName, "/tmp/%s", ent->d_name);
				fd = open(readFileName, O_RDONLY);

				read(fd, buffer, sizeof(int));
				//printf("esto leyo: %i\n", (int*)buffer);
				int size = *((int*)buffer);
				read(fd, ((char*)buffer) + sizeof(int), size - sizeof(int));
				if (!remove(readFileName))
					printf("borrado: %s\n", readFileName);
				else
					printf("no borrado: %s\n", readFileName);
				close(fd);
				return;
			}
			reading = 0;
			closedir (dir);
		}
	} else { //client
		if (aux) {
			sigemptyset (&mask);
			sigaddset (&mask, SIGUSR1);
			sigprocmask (SIG_BLOCK, &mask, &oldmask);
			aux = 0;
		}
		sigsuspend(&oldmask);
		fd = open(readFileName, O_RDONLY);
		read(fd, buffer, sizeof(int));
		int size = *((int*)buffer);
		read(fd, ((char*)buffer) + sizeof(int), size - sizeof(int));
		if (!remove(readFileName))
			printf("borrado: %s\n", readFileName);
		else
			printf("no borrado: %s\n", readFileName);
		close(fd);
	}
	return;
}

leftStringMatch(char * begin, char * string) {
	int match = 1;
	while (*begin != 0 && match) if (*(begin++) != *(string++)) match = 0;
	return match;
}

void mypause(int sign) {
	signal(SIGUSR1, mypause);
	printf("entro a la señal\n");
}

void handOff(int sig){
	if(is_server){
		close(fd);
		if(remove("tmp/server_pid"))
			printf("Server stopped correctly\n");
		else
			printf("Server's communication file could not be removed\n");
	}else{
		close(fd);
		if(remove(readFileName) &&
			remove(writeFileName))
			printf("Communication files where removed\n");
	}

	exit(0);
}