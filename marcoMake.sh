gcc client.c APICli.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h -lrt -pthread -o shmCli


gcc server.c APIServer.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h -lrt -pthread -o shmServer
