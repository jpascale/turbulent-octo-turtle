gcc client.c APICli.c communicationSockets.c -lrt -pthread -o sockCli.o
gcc server.c APIServer.c communicationSockets.c sqlite/sqlite3.c sqlLib.c -lrt -pthread -ldl -o sockServ.o