#include <stdio.h> 
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("falla connect");
        return 1;
    }
     
    printf("Connected\n");
     
    while(1)
    {
        //printf("Enter message: ");
        if (read(0, message, sizeof message) > 0){
         
        	if(send(sock , message , strlen(message) , 0) < 0)
        	{
            	puts("Send failed");
            	return 1;
        	}
        
        	if(recv(sock , server_reply , 2000 , 0) < 0)
        	{
            	puts("recv failed");
            	break;
        	}
         
        	printf("Server: %s", server_reply);

        	memset(message, 0, sizeof message);
        	memset(server_reply, 0, sizeof server_reply);
        }
    }
     
    close(sock);
    return 0;
}