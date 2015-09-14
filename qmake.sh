gcc client.c APICli.c communicationQueue.c -lrt -pthread -o qCli.o
gcc server.c APIServer.c communicationQueue.c sqlite/sqlite3.c sqlLib.c -lrt -pthread -ldl -o qServ.o