gcc client.c APICli.c sharedFunctions.h  communication.h communicationSharedMem.c  datagram.h -lrt -pthread -o shmCli


gcc server.c APIServer.c sharedFunctions.h  communication.h communicationSharedMem.c  datagram.h -lrt -pthread -o shmServer
