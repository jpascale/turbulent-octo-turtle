#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./datagram.h"

#define NAME "/tmp/fifo"

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

void 
chau(int sig)
{
	printf("Padre recibe SIGPIPE y termina\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	int pid, n;

	connect();

	while(7 == 7){ //arreglar
		receiveData()	
	}
	
	if ( access(NAME, 0) == -1 && mknod(NAME, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod");
	
	fd = open(NAME, O_RDONLY);
	while ( (n = read(fd, buf, sizeof buf)) > 0 )
	{
		printf("Hijo lee del pipe: %.*s", n, buf);
		if ( memcmp(buf, "end", 3) == 0 )
			exit(1);
	}
	printf("Hijo termina\n");
}
