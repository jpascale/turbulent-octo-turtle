#include <stdio.h> 
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[100], server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    int w;

    while(1){

        if (read(0, message, sizeof message) > 0){

         	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
         		printf ("Socket error\n");
         	}

        	if ((w=connect(sock, (struct sockaddr *)&server, sizeof(server)))>= 0){

        		if(send(sock, message, sizeof message, 0) < 0){
            		puts("Send failed");
            		return 1;
        		}else{
        			printf("sent.\n");
        		}
        
        		if(recv(sock, server_reply, 2000, 0) < 0){
            		puts("recv failed");
            		break;
        		}
         
        		printf("Server: %s", server_reply);

        		memset(message, 0, sizeof message);
        		memset(server_reply, 0, sizeof server_reply);

        		close(sock);
        		printf("DEBUG: Desconectado.\n");
        	}

        }
    }
     
    close(sock);
    return 0;
}