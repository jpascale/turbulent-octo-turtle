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

#define DEBUG true

/*
**		Global declares
*/
//TODO: Merge if possible
//SERVER
bool is_server;

int socket_desc;
int client_sock; 
int c;
int * new_sock;
struct sockaddr_in server, client;

//CLIENT
int sock;
struct sockaddr_in server;
char message[100];
char server_reply[2000];

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
        printf("Could not create socket\n");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(port);
    
    //This allows another socket to bind the ip, port, allows instant server reboot
    int enable = 1;
	if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    	perror("setsockopt(SO_REUSEADDR) failed");

    //Bind
    if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("bind failed. Error");
        return;
    }

    //Listen
    listen(socket_desc , 3);
     
    c = sizeof(struct sockaddr_in);

    return;
}

void srv_receive_data(Connection * connection, Datagram * sdData){

 	if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
         
    //__________Receive incoming data____________
        if (DEBUG)
        	printf("Accepted %d socket.\n", client_sock);

    	int read_size;

     	Datagram data;

     	if (DEBUG)
     		printf("Esperando para leer\n");

    	if ((read_size = recv(client_sock, &data, sizeof data, 0)) > 0){

    		//Poner en send data
    		if (DEBUG)
    			printf("Dat income: %s", data.data.m.title);
    		memcpy(sdData, &data, sizeof data);
    		
    		return;
    	}
     
    	if (read_size == 0){
        	if (DEBUG)
        		printf("Client %d disconnected\n", client_sock);
        	fflush(stdout);
    	} else if (read_size == -1){
        	perror("recv failed");
    	}

    	if (client_sock < 0){
    		printf ("El puerto esta siendo usado.\n");
    		exit(1);
    	}
    }
     
    if (client_sock < 0)
    {
        perror("El puerto esta siendo usado.\n");
        return;
    }
    

    return;
     
}

void srv_send_data(Connection * coneccion, Datagram * sdData){
	
	Datagram data;
	memcpy(&data, sdData, sizeof data);

	write(client_sock, &data, sizeof data);
    close(client_sock);
}

void clt_init_channel(void){
	is_server = false;

	sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");
     
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}

void clt_send_data(Connection * connection, Datagram * sdData){
	
	Datagram data;
	memcpy(&data, sdData, sizeof data);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf ("Socket error\n");
    }

    if (connect(sock, (struct sockaddr *)&server, sizeof(server))>= 0){
   		
   		if(send(sock, &data, sizeof data, 0) < 0){
      		puts("Send failed");
       		return;
   		}else{
   			printf("Sent.\n");
       	}
        
        if(recv(sock, &data, sizeof data, 0) < 0){
            puts("recv failed");
            return;
        }
         
        if (DEBUG)
        	printf("DEBUG Server: %s", data.data.m.title);

       	close(sock);
       	printf("DEBUG: Desconectado.\n");
    }
}

void clt_receive_data(Connection * connection, Datagram * sdData){

}