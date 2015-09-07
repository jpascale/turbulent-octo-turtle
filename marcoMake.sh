gcc client.c APICli.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h sqlite/sqlite3.c -o shmCli -lrt -pthread -lpthread -ldl


gcc server.c APIServer.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h sqlite/sqlite3.c -o shmServer -lrt -pthread -lpthread -ldl
