gcc client.c APICli.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h -lrt -pthread -o shmCli


gcc -o shmServer server.c APIServer.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h sqlLib.h sqlLib.c ./sqlite/sqlite3.c ./sqlite/sqlite3.h -lrt -pthread -lpthread -ldl
