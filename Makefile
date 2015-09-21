fifo: client.c APICli.c communicationFilesAndSignals.c server.c APIServer.c

	gcc communicationFifo.c APICli.c client.c -o client
	gcc communicationFifo.c APIServer.c server.c sqlite/sqlite3.c sqlLib.c -o server -lpthread -ldl

files: client.c APICli.c communicationFilesAndSignals.c server.c APIServer.c

	gcc communicationFilesAndSignals.c APICli.c client.c -o client
	gcc communicationFilesAndSignals.c APIServer.c server.c sqlite/sqlite3.c sqlLib.c -o server -lpthread -ldl

shared:

	gcc client.c APICli.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h -lrt -pthread -o client

	gcc -o server server.c APIServer.c communicationSharedMem.c datagram.h communication.h sharedFunctions.h sqlLib.h sqlLib.c ./sqlite/sqlite3.c ./sqlite/sqlite3.h -lrt -pthread -lpthread -ldl

queues:

	gcc client.c APICli.c communicationQueue.c -lrt -pthread -o client
	gcc server.c APIServer.c communicationQueue.c sqlite/sqlite3.c sqlLib.c -lrt -pthread -ldl -o server

sockets:

	gcc client.c APICli.c communicationSockets.c -lrt -pthread -o client
	gcc server.c APIServer.c communicationSockets.c sqlite/sqlite3.c sqlLib.c -lrt -pthread -ldl -o server