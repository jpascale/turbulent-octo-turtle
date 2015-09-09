#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./datagram.h"

#define NAME "/tmp/fifo"
#define MAGIC_NUMBER_TO_AVOID_WARNINGS 12



void 
chau(int sig)
{
	printf("Padre recibe SIGPIPE y termina\n");
	exit(1);
}

int
main2(int argc, char **argv)
{
	int fd, i,n ;
	char* buf;
	
	if ( access(NAME, 0) == -1 && mknod(NAME, S_IFIFO|0666, 0) == -1 )
	perror("mknod");
	exit(1);
	
	fd = open(NAME, O_RDONLY);
	while ( (n = read(fd, buf, sizeof buf)) > 0 )
	{
		printf("Hijo lee del pipe: %.*s", n, buf);
		if ( memcmp(buf, "end", 3) == 0 )
			exit(1);
	}
	printf("Hijo termina\n");
}
