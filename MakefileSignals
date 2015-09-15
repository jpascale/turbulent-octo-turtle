all: client.c APICli.c communicationFilesAndSignals.c server.c APIServer.c
	gcc communicationFilesAndSignals.c APICli.c client.c -o client
	gcc communicationFilesAndSignals.c APIServer.c server.c sqlite/sqlite3.c sqlLib.c -o server -lpthread -ldl
