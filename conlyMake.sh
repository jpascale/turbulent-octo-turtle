gcc -o conly client.c ConlyAPI.c datagram.h communication.h sharedFunctions.h sqlLib.h sqlLib.c ./sqlite/sqlite3.c ./sqlite/sqlite3.h -lrt -pthread -lpthread -ldl
