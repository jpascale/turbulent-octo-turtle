all: client.c APICli.c communicationFifo.c server.c APIServer.c
	gcc communicationFifo.c APICli.c client.c -o client
	gcc communicationFifo.c APIServer.c server.c sqlite/sqlite3.c -o server -lpthread -ldl
