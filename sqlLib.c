#include "sqlite/sqlite3.h"
#include "sqlLib.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>	



typedef enum {GET_MOVIE_LIST, GET_MOVIE_DETAILS, GET_MOVIE_SHOWS, GET_SHOW_SEATS, BUY_TICKET_CHECK_AVAILABILITY, BUY_TICKET_CHECK_SEAT, BUY_TICKET_CHECK_SHOW,
				UNDO_BUY_TICKET, ADD_SHOW, REMOVE_SHOW, ADD_MOVIE, REMOVE_MOVIE} functions;

sqlite3 * db;
int rc, auxResp;
char *errMsg = 0, *sql_create_rooms, *sql_insert, * sql_get_movie_list,
 * sql_create_movies, * sql_create_shows, *sql_create_seats, *sql_create_tickets, *answer;
char sql_query[256];

static int callback(void * function, int argc, char **argv, char **azColName){
   int i, k;
  		printf("entro %i\n", (int)function );
   switch((int)function){
		case GET_MOVIE_LIST:
			printf("entro a get_movie\n" );
	   		for(i=0; i<argc; i++){
      			k = sprintf(answer, "%s;", argv[i] ? argv[i] : "NULL");
   				answer+=k;
			break;
			
		case GET_MOVIE_DETAILS:
			for(i=0; i<argc; i++){
				k = sprintf(answer, "%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
				answer+=k;
			}
			break;
			
		case GET_MOVIE_SHOWS:
			k = sprintf(answer, "%s: %s, %s: %s\n ", azColName[0], argv[0] ? argv[0] : "NULL", azColName[1], argv[1] ? argv[1] : "NULL");
   			answer+=k;
			break;	
		   
		case GET_SHOW_SEATS:
			k = sprintf(answer, "%s\n", argv[0]);
			answer+=k;
			break;
   		
		case BUY_TICKET_CHECK_AVAILABILITY:
			auxResp = 1;
			break;
		
		case BUY_TICKET_CHECK_SEAT:
			auxResp = 1;
			break;
		
		case BUY_TICKET_CHECK_SHOW:
			auxResp = 1;
			break;
   		}
   }
   
   return 0;
}

void SQLgetMovieList(char * buffer){
	answer = buffer;
	sprintf(sql_query, "select title, movieID from movies;");
	rc = sqlite3_exec(db, sql_query, callback, (void*) GET_MOVIE_LIST, &errMsg);
	if(rc)
		printf("error: %s", sqlite3_errmsg(db));
	else
		printf("no hay error");
	return;
}

void SQLgetMovieDetails(char * buffer, int movieID){
	answer = buffer;
	sprintf(sql_query, "select title, desc, length, movieID from movies where (movies.movieID = %i);", movieID); 
	rc = sqlite3_exec(db, sql_query, callback, (void*) GET_MOVIE_DETAILS, &errMsg);
	return;
}

void SQLgetMovieShow(char * buffer, int movieID){
	answer = buffer;
	sprintf(sql_query, "select time, showID from shows where (shows.movieID = %i);", movieID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) GET_MOVIE_SHOWS, &errMsg);
	return;
}

void SQLgetShowSeats(char * buffer, int showID){
	answer = buffer;
	sprintf(sql_query, "select seatNum from tickets where (tickets.showID = %i);", showID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) GET_SHOW_SEATS, &errMsg);
	return;
}

void SQLbuyTicket(char * buffer, int showID, int asiento, char* nombre){
	answer = buffer;
	auxResp = 0;
	sprintf(sql_query, "select count(*) from tickets where (tickets.showID = %i and tickets.seatNum = %i);", showID, asiento);
	rc = sqlite3_exec(db, sql_query, callback, (void*) BUY_TICKET_CHECK_AVAILABILITY, &errMsg);
	if(auxResp){
		sprintf(answer, "Ticket no disponible");
		return;
	}
	sprintf(sql_query, "select count(*) from shows where (shows.showID = %i);", showID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) BUY_TICKET_CHECK_SHOW, &errMsg);
	auxResp = 0;
	if(!auxResp){
		sprintf(answer, "ShowID invalido");
		return;
	}
	sprintf(sql_query, "select count(*) from seats, shows where (shows.roomID = seats.roomID and seats.seatNum = %i);", asiento);
	rc = sqlite3_exec(db, sql_query, callback, (void*) BUY_TICKET_CHECK_SEAT, &errMsg);
	auxResp = 0;	
	if(!auxResp){
		sprintf(answer, "Asiento invalido");
		return;
	}
	sprintf(sql_query, "insert into tickets values (%i, %i, '%s');", asiento, showID, nombre);
	rc = sqlite3_exec(db, sql_query, callback, (void*) BUY_TICKET_CHECK_SEAT, &errMsg);
	sprintf(answer, "Compra realizada");
	return;
}





	//funciones///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	

	
	//devuelve los ocupados

	

	/*
	sprintf(sql_undo_buy_ticket, "delete from tickets where (tickets.name = '%s' and tickets.ticketID = %i);",)
	
	sprintf(sql_add_show, "insert into show values (%i, %i, %i, %i);",,,,);
	
	sprintf(sql_remove_show, "delete from shows where (shows.showID = %i);",);
	
	sprintf(sql_add_movie, "insert into movie values (%i, %i, '%s', '%s');",,,,);
	
	sprintf(sql_remove_show, "delete from shows where (shows.showID = %i);",);
	*/
		
	//creando tablas
	
	


void setUpDB(){

	rc = sqlite3_open("test.db", &db);
	
	sql_create_rooms = "CREATE TABLE rooms("
         "roomID 	int	PRIMARY KEY     NOT NULL AUTO_INCREMENT,"
         "maxCap 	int	not null);";
		 
	sql_create_movies =	 "create table movies("
		 "movieID	int primary key not null AUTO_INCREMENT,"
		 "length	int not null,"
		 "title		varchar(128) not null,"
		 "desc		varchar(1024) not null);";
		 
	sql_create_shows =	 "create table shows("
		 "showID	int primary key not null AUTO_INCREMENT,"
		 "time		int not null,"
		 "roomID	int not null,"
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
		 "on delete cascade"
		 "on update );" ;
		 
	sql_create_tickets =	 "create table tickets(" 
		 "ticketID	int not null primary key AUTO_INCREMENT," 
		 "seatNum	int not null," 
		 "showID	int not null,"
		 "name  	varchar(64),"
		 "unique (seatNum, showID)," 
		 "foreign key (showID) references shows(showID)" 
		 "on delete cascade);";
	
	//////////////////////////
	
	sql_insert = "insert into movies (movieID, length, title, desc)"
				"values (12, 120, 'volver al futuro', 'vuelven al futuro');";
				

	printf("creando tablas:\n")	;	
	rc = sqlite3_exec(db, sql_create_rooms, callback, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
	
	rc = sqlite3_exec(db, sql_create_movies, callback, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   
   rc = sqlite3_exec(db, sql_create_shows, callback, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   
   rc = sqlite3_exec(db, sql_create_seats, callback, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   	}else{
      fprintf(stdout, "Table created successfully\n");
   	}
	
	rc = sqlite3_exec(db, sql_create_tickets, callback, 0, &errMsg);
	if( rc != SQLITE_OK ){
   		fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
	//termina de crear tablas
	
	//insertando cosas
	printf("insertando registros:\n");
	rc = sqlite3_exec(db, sql_insert, callback, 0, &errMsg);
	if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   }else{
      fprintf(stdout, "Records created successfully\n");
   }
   //termina de insertar cosas
	
}
