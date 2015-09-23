#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#include "./datagram.h"

#define SERVER_PID_PATH "/tmp/server_pid"

int leftStringMatch(char * begin, char * string);
void mypause(int sign);

int is_server, reading = 0, fd, server_fd,server_pid, aux = 1;
DIR *dir;
struct dirent *ent;
sigset_t mask, oldmask;
char auxS[32], writeFileName[32], readFileName[32];

void initChannel(int bool_server) {
	is_server = bool_server;
	signal(SIGUSR1, mypause);
	if (is_server) { //server
		remove(SERVER_PID_PATH);
		fd = open(SERVER_PID_PATH, O_CREAT | O_WRONLY, 777);
		server_pid = getpid();
		write(fd, &server_pid, sizeof(int));
		close(fd);
		reading = 0;
	} else { //client
		server_fd = -1;
		if(!access(SERVER_PID_PATH, F_OK)){
			printf("hay server\n");
			server_fd = open(SERVER_PID_PATH, O_RDONLY);
			read(server_fd, auxS, sizeof(int));
			close(server_fd);
			server_pid = *((int*)auxS);
			sprintf(writeFileName, "/tmp/request_%d", getpid());
			sprintf(readFileName, "/tmp/response_%d", getpid());
		}else
			printf("no hay server\n");
	}
}

int sendData(Connection * connection, Datagram * params) {

	if (is_server) {
		sprintf(writeFileName, "/tmp/response_%i", connection->sender_pid);
		fd = open(writeFileName, O_CREAT | O_WRONLY, 777);
		write(fd, params, *(int*)params);
		kill(connection->sender_pid, SIGUSR1);
		close(fd);
	} else {
		printf("server_fd: %i\n", server_fd);
		if(server_fd<0 || access(SERVER_PID_PATH, F_OK)){	
			if(access(SERVER_PID_PATH, F_OK))
				return -1;
			initChannel(is_server);
		}

		fd = open(writeFileName, O_CREAT | O_WRONLY, 777);
		write(fd, params, *(int*)params);
		kill(server_pid, SIGUSR1);
		close(fd);
	}

}

void receiveData(Connection * sender, Datagram * buffer) {
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

int leftStringMatch(char * begin, char * string) {
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
		if(!remove(SERVER_PID_PATH))
			printf("Server stopped correctly\n");
		else
			printf("Server's communication file could not be removed\n");
	}else{
		close(fd);
		if(!remove(readFileName) && !remove(writeFileName))
			printf("Communication files where removed\n");
		else
			printf("Client's communication files could not be removed\n");
	}

	exit(0);
}