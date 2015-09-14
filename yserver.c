#include "datagram.h"
#include "communication.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void){

	initChannel(1);
	
	Connection conn;
	Datagram data;

	while (1){
		receiveData(&conn, &data);
    	printf("Dat income: %s", data.data.text);
		sendData(&conn, &data);
	}
}