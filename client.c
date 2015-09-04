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
	int fd;
	Datagram * data = malloc(1000);
	data->data = 
	connect();
	
	getMovieList();
	
	char buf[200];
/*
	if ( access(NAME, 0) == -1 && mknod(NAME, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod");

	signal(SIGPIPE, chau);
	fd = open(NAME, O_WRONLY);
	while ( (n = read(0, buf, sizeof buf)) > 0 )
	{
		printf("Padre escribe en el pipe: %.*s", n, buf);
		write(fd, buf, n);
	}
	printf("Padre termina\n");
*/
}
