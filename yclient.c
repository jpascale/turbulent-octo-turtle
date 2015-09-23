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

    while (1) {

        //Reset buffer
        memset(&data, 0, sizeof data);

        if (read(0, data.data.text, sizeof data.data.text) > 0) {

            data.size = sizeof data;
            data.opcode = 0;

            sendData(&conn, &data);
            receiveData(&conn, &data);

            printf("DEBUG Server: %s", data.data.text);

        }
    }

    return 0;
}