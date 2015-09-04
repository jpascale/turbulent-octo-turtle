gcc client.c APICli.c communicationSharedMem.c -lrt -pthread -o shmCli


gcc server.c APIServer.c communicationSharedMem.c -lrt -pthread -o shmServer
