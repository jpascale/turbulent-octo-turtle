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
**      Private function declares
*/
void srv_init_channel(void);
void clt_init_channel(void);
int srv_send_data(Connection * connection, Datagram * sdData);
void srv_receive_data(Connection * connection, Datagram * sdData);
int clt_send_data(Connection * connection, Datagram * sdData);
void clt_receive_data(Connection * connection, Datagram * sdData);


/*
**      Defines
*/
typedef enum { false, true } bool;
#define MAX_RDATA_SIZE 3000

#define server_ip "127.0.0.1"
#define port 8000

#define DEBUG false

/*
**      Global declares
*/

bool is_server;

int socket_desc;
int client_sock;
int c;
int * new_sock;

//server is used to load server config and client to receive incoming connection config
struct sockaddr_in server, client;
int sock;

/*
**      Module functions
*/

/*
**  Init the communication channel
**  Receives 1 for server and 0 for client
*/
void initChannel(int bool_server) {

    switch (bool_server) {

    case true:
        srv_init_channel();
        break;

    case false:
        clt_init_channel();
        break;
    }
}

int sendData(Connection * connection, Datagram * params) {
    switch (is_server) {
    case true:
        return srv_send_data(connection, params);
        break;

    case false:
        return clt_send_data(connection, params);
        break;
    }
}

void receiveData(Connection * connection, Datagram * params) {
    switch (is_server) {
    case true:
        srv_receive_data(connection, params);
        break;

    case false:
        clt_receive_data(connection, params);
        break;
    }
}

/*
**      Private module functions
*/

/*
**      srv_init_channel: Set de server boolean true and
**      load server socket config
*/
void srv_init_channel(void) {

    is_server = true;

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket.\n");
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
    if (bind(socket_desc, (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("bind failed. Error");
        return;
    }

    //Listen
    listen(socket_desc, 3);

    c = sizeof(struct sockaddr_in);

    return;
}

/*
**      srv_receive_data: Accept incoming connection, read from
**      the accepted socket and copy the data into the passing structure.
*/
void srv_receive_data(Connection * connection, Datagram * sdData) {

    if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {

        //__________Receive incoming data____________
        if (DEBUG)
            printf("Accepted %d socket.\n", client_sock);

        int read_size;
        void * data = calloc(MAX_RDATA_SIZE, 1);

        if (DEBUG)
            printf("Esperando para leer\n");

        if ((read_size = recv(client_sock, data, MAX_RDATA_SIZE, 0)) > 0) {
            if (DEBUG) {
                printf("DEBUG: Reading size: %d\n", read_size);
                printf("DEBUG: Structure size: %d\n", *((int*)data));
            }
            memcpy(sdData, data, read_size);
        }

        if (read_size == 0) {
            if (DEBUG)
                printf("Client %d disconnected\n", client_sock);
            fflush(stdout);
        } else if (read_size == -1) {
            perror("recv failed");
        }

        if (client_sock < 0) {
            printf ("El puerto esta siendo usado.\n");
            exit(1);
        }
        free(data);
    }

    if (client_sock < 0)
    {
        perror("El puerto esta siendo usado.\n");
        return;
    }


    return;

}

/*
**      srv_send_data: Send data to current connection
**      and close the socket.
*/
int srv_send_data(Connection * coneccion, Datagram * sdData) {

    void * data = calloc(sdData -> size, 1);
    memcpy(data, sdData, sdData -> size);

    write(client_sock, data, sdData -> size);
    close(client_sock);

    free(data);

    return 0;
}

/*
**      clt_init_channel: Set server boolean to false
**      and load client config
*/
void clt_init_channel(void) {
    is_server = false;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("Could not create socket");
    }

    if (DEBUG)
        printf("Socket created\n");

    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
}


/*
**      clt_send_data: Copy arguments to buffer, establish connection
**      with server, and send data.
*/
int clt_send_data(Connection * connection, Datagram * sdData) {

    void * data = calloc(sdData -> size, 1);
    memcpy(data, sdData, sdData -> size);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf ("Socket error\n");
    }

    int c;
    if ((c = connect(sock, (struct sockaddr *)&server, sizeof(server))) >= 0) {
        if (send(sock, data, *((int * )data), MSG_NOSIGNAL) < 0) {
            //puts("Send failed");
            return -1;
        } else {
            if (DEBUG)
                printf("Sent.\n");
        }
    }else
        return -1;

    free(data);
    return 0;
}


/*
**      clt_receive_data: Read from established connection, close
**      the socket and return data. 
*/
void clt_receive_data(Connection * connection, Datagram * sdData) {

    void * data = calloc(MAX_RDATA_SIZE, 1);
    int bff_size;

    if ((bff_size = recv(sock, data, MAX_RDATA_SIZE, 0)) < 0) {
        puts("Servidor no disponible.\n");
        return;
    }

    close(sock);

    memcpy(sdData, data, bff_size);

    if (DEBUG)
        printf("DEBUG: Desconectado.\n");

    free(data);

    return;
}

void handOff(int sig) {
    printf("Termina por señal %d\n", sig);
    exit(0);
}