#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#include "./datagram.h"

int leftStringMatch(char * begin, char * string);

int is_server, reading = 0, fd;
DIR *dir;
struct dirent *ent;	

initChannel(int bool_server){
	is_server = bool_server;
	if(is_server){
		dir = opendir ("/tmp/");
	}
}

sendData(Connection * connection, Datagram * params){
	
}

receiveData(Connection * sender, Datagram * buffer){
	if(is_server){
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
	}
}
leftStringMatch(char * begin, char * string){
	int match = 1;
	while (*begin!=0 && match) if(*(begin++)!=*(string++)) match = 0;
	return match;
}