#include "sharedFunctions.h"
#include "communication.h"
#include "datagram.h"
#include "sqlite/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

Datagram data;
Connection sender;

char answer[4096];
char * auxAnswer;
static int callback_get_movie_list(void *NotUsed, int argc, char **argv, char **azColName){
   int i, k;
   for(i=0; i<argc; i++){
      	k = sprintf(auxAnswer, "%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   		auxAnswer+=k;
   }
   printf("entro al callback\n");
   return 0;
}

void main(){
	
	sqlite3 * db;
	int rc;
	char *errMsg = 0, *sql_create_rooms, *sql_insert, * sql_get_movie_list,
	 * sql_create_movies, * sql_create_shows, *sql_create_seats, *sql_create_tickets;
	rc = sqlite3_open("test.db", &db);
	
	sql_create_rooms = "CREATE TABLE rooms("
         "roomID 	int	PRIMARY KEY     NOT NULL,"
         "maxCap 	int	not null);";
		 
	sql_create_movies =	 "create table movies("
		 "movieID	int primary key not null,"
		 "length	int not null,"
		 "title		varchar(128) not null,"
		 "desc		varchar(1024) not null);";
		 
	sql_create_shows =	 "create table shows("
		 "showID		int primary key not null,"
		 "time		int not null,"
		 "roomID		int not null,"
		 "movieID	int not null," 
		 "foreign key (roomID) references rooms(roomID)" 
		 "on delete cascade," 
		 "foreign key (movieID) references movies(movieID)" 
		 "on delete cascade," 
		 "unique (roomID, movieID));";
		 
	sql_create_seats =	 "create table seats(" 
		 "seatNum	int not null," 
		 "roomID		int not null," 
		 "primary key (seatNum, roomID)," 
		 "foreign key (roomID) references rooms(roomID)" 
		 "on delete cascade);" ;
		 
	sql_create_tickets =	 "create table tickets(" 
		 "ticketID	int not null primary key," 
		 "seatNum	int not null," 
		 "showID		int not null," 
		 "unique (seatNum, showID)," 
		 "foreign key (showID) references shows(showID)" 
		 "on delete cascade);";
	
	sql_get_movie_list = "select * from movies;";
	
	sql_insert = "insert into movies (movieID, length, title, desc)"
				"values (12, 120, 'volver al futuro', 'vuelven al futuro');";
	
	//creando tablas
	printf("creando tablas:\n")	;	
	rc = sqlite3_exec(db, sql_create_rooms, callback_get_movie_list, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
	
	rc = sqlite3_exec(db, sql_create_movies, callback_get_movie_list, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   
   rc = sqlite3_exec(db, sql_create_shows, callback_get_movie_list, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   
   rc = sqlite3_exec(db, sql_create_seats, callback_get_movie_list, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
	
	rc = sqlite3_exec(db, sql_create_tickets, callback_get_movie_list, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
	//termina de crear tablas
	
	//insertando cosas
	printf("insertando registros:\n");
	rc = sqlite3_exec(db, sql_insert, callback_get_movie_list, 0, &errMsg);
	if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }
   //termina de insertar cosas
	auxAnswer = answer;
	
	rc = sqlite3_exec(db, sql_get_movie_list, callback_get_movie_list, (void*) auxAnswer, &errMsg);
	
	printf("a ver:\n%s", answer);



/*	
	initChannel(1);
	printf("Server conectado\n");


	printf("Cargado\n");
	while(1){
		receiveData(&sender, &data);
		switch(fork()){
			case -1:
				//error
			case 0:
				printf("creo que el pid del que me envio esto es: %d\n", data.client_pid);
				printf("Recive:%i \n",data.client_pid);
				processData(&sender, &data);
				exit(0);
		}
	}
*/
}

void processData(Connection * sender, Datagram * data){

	switch(data->opcode){
	
		case GET_MOVIE_LIST:
			printf("Llama consultar cartelera de server\n");
			break;
		
		default:
			printf("Comando no soportado\n");
	}
	sender->sender_pid=data->client_pid;
	
	sendData(sender,data);
}
