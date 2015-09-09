#include "datagram.h"
#include "communication.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

/*
**		Private function declares
*/
void srv_init_channel(void);
void clt_init_channel(void);
void create_ioqueue(void);
void srv_send_data(Connection * connection, Datagram * sdData);
void srv_receive_data(Connection * connection, Datagram * sdData);
void clt_send_data(Connection * connection, Datagram * sdData);
void clt_receive_data(Connection * connection, Datagram * sdData);


/*
**		Defines
*/
typedef enum { false, true } bool;
#define MAX_RDATA_SIZE 3000

#define server_ip "127.0.0.1"
#define port 8000

/*
**		Global declares
*/
bool is_server;

int socket_desc;
int client_sock; 
int c;
int * new_sock;
struct sockaddr_in server, client;


/*
**		Module functions
*/

/*
**	Init the communication channel
**	Receives 1 for server and 0 for client
*/
void initChannel(int bool_server){

	switch (bool_server){

		case true:
			srv_init_channel();
			break;

		case false:
			clt_init_channel();
			break;
	}
}

void sendData(Connection * connection, Datagram * params){
	switch (is_server){
		case true:
			srv_send_data(connection, params);
			break;

		case false:
			clt_send_data(connection, params);
			break;
	}
}

void receiveData(Connection * connection, Datagram * params){
	switch (is_server){
		case true:
			srv_receive_data(connection, params);
			break;

		case false:
			clt_receive_data(connection, params);
			break;
	}
}

/*
**		Functions only declared in this module
*/

void srv_init_channel(void){

	is_server = true;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1){
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
     
    //Bind
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("bind failed. Error");
        return;
    }

    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    c = sizeof(struct sockaddr_in);
        
    //Accept an incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    return;
}

void srv_receive_data(Connection * connection, Datagram * sdData){
 
 	if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){

        puts("Connection accepted");
         
        new_sock = malloc(4);
        *new_sock = client_sock;
         
    //__________Receive incoming data____________
        
        //socket descriptor
    	int sock = *(int*)new_sock;
    	int read_size;
    	char * message;
    	char client_message[2000];
     	memset(client_message, 0, sizeof client_message);

    	if ((read_size = recv(sock , client_message , 2000 , 0)) > 0){

    		//Poner en send data
    		printf("Dat income: %s", client_message);
        	write(sock , client_message , strlen(client_message));
    	}
     
    	if (read_size == 0){
        	printf("Client disconnected\n");
        	fflush(stdout);
    	} else if (read_size == -1){
        	perror("recv failed");
    	}
         
    	//Free the socket pointer
    	close(client_sock);
    	free(new_sock);
    	//free(socket_desc);
    //___________________________________________
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return;
    }
     
    return;
     
}

void srv_send_data(Connection * coneccion, Datagram * sdData){

}

void clt_init_channel(void){
	is_server = false;
}

void clt_send_data(Connection * connection, Datagram * sdData){

}

void clt_receive_data(Connection * connection, Datagram * sdData){

}