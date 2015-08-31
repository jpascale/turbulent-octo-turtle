#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "./datagram.h"

void sendData(Connection * connection, int size, void * params);
void receiveData(Connection * sender, int size, void * buffer);
void initChannel(int bool_server);

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
	char *name = "/tmp/fifo";
	char buf[200];

	if ( access(name, 0) == -1 && mknod(name, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod");

	switch (pid = fork())
	{
		case -1:
			fatal("Error fork");
			break;

		case 0: /* hijo */
			fd = open(name, O_RDONLY);
			while ( (n = read(fd, buf, sizeof buf)) > 0 )
			{
				printf("Hijo lee del pipe: %.*s", n, buf);
				if ( memcmp(buf, "end", 3) == 0 )
					break;
			}
			printf("Hijo termina\n");
			break;
		
		default: /* padre */
			signal(SIGPIPE, chau);
			fd = open(name, O_WRONLY);
			while ( (n = read(0, buf, sizeof buf)) > 0 )
			{
				printf("Padre escribe en el pipe: %.*s", n, buf);
				write(fd, buf, n);
			}
			printf("Padre termina\n");
			break;
	}
	
	
}
