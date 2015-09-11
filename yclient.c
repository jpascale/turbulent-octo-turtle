#include "datagram.h"
#include "communication.h"

#include <stdio.h> 
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
 
int main(int argc , char *argv[])
{

	Datagram data;
	Connection conn;

	initChannel(0);

    while(1){

        if (read(0, data.data.m.title, sizeof data.data.m.title) > 0){
			
			sendData(&conn, &data);
			receiveData(&conn, &data);
			
      		printf("DEBUG Server: %s", data.data.m.title);
      	
			memset(data.data.m.title, 0, sizeof data.data.m.title);
        }
    }
     
    return 0;
}