all: client server

	gcc communicationFifo.c APICli.c client.c -o client
	gcc communicationFifo.c APIServer.c server.c -o server
