#include "sqlite/sqlite3.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>	
#include "sqlLib.h"

typedef enum {GET_MOVIE_LIST, GET_MOVIE_DETAILS, GET_MOVIE_SHOWS, GET_SHOW_SEATS, COUNT_CHECK,
				UNDO_BUY_TICKET, ADD_SHOW, REMOVE_SHOW, ADD_MOVIE, REMOVE_MOVIE} functions;

sqlite3 * db;
int rc, auxResp, type;
char *errMsg = 0, *sql_create_rooms, *sql_insert, * sql_get_movie_list,
 * sql_create_movies, * sql_create_shows, *sql_create_seats, *sql_create_tickets, *answer;
char sql_query[256];

static int callback(void * function, int argc, char **argv, char **azColName){
   int i, k;
   switch(*((int*)function)){
		case GET_MOVIE_LIST:
	   		for(i=0; i<argc; i++){
      			k = sprintf(answer, "%s;", argv[i] ? argv[i] : "NULL");
   				answer+=k;
			}
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
   		
		case COUNT_CHECK:
			printf("count: %s\n", argv[0]);
			if(argv[0][0]!='0') auxResp = 1;
			break;
			
   		}
		   
   return 0;
}

void SQLgetMovieList(char * buffer){
	answer = buffer;
	type = GET_MOVIE_LIST;
	sprintf(sql_query, "select title, movieID from movies;");
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	return;
}

void SQLgetMovieDetails(char * buffer, int movieID){
	answer = buffer;
	type = GET_MOVIE_DETAILS;
	sprintf(sql_query, "select title, desc, length, movieID from movies where (movies.movieID = %i);", movieID); 
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	return;
}

void SQLgetMovieShow(char * buffer, int movieID){
	answer = buffer;
	type = GET_MOVIE_SHOWS;
	sprintf(sql_query, "select time, showID from shows where (shows.movieID = %i);", movieID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	return;
}

void SQLgetShowSeats(char * buffer, int showID){
	answer = buffer;
	type = GET_SHOW_SEATS;
	sprintf(sql_query, "select seatNum from tickets where (tickets.showID = %i);", showID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);	
	return;
}

void SQLbuyTicket(char * buffer, int showID, int asiento, char* nombre){
	answer = buffer;
	type = COUNT_CHECK;
	auxResp = 0;
	sprintf(sql_query, "select count(*) from tickets where (tickets.showID = %i and tickets.seatNum = %i);", showID, asiento);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);	
	if(auxResp){
		sprintf(answer, "Ticket no disponible");
		return;
	}
	type = COUNT_CHECK;
	sprintf(sql_query, "select count(*) from shows where (shows.showID = %i);", showID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	auxResp = 0;
	if(!auxResp){
		sprintf(answer, "ShowID invalido");
		return;
	}
	type = COUNT_CHECK;
	sprintf(sql_query, "select count(*) from rooms, shows where (shows.showID = %i and shows.roomID = rooms.roomID and rooms.maxCap > %i);", showID, asiento);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	auxResp = 0;	
	if(!auxResp){
		sprintf(answer, "Asiento invalido");
		return;
	}
	sprintf(sql_query, "insert into tickets values (%i, %i, '%s');", asiento, showID, nombre);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	sprintf(answer, "Compra realizada");
	return;
}

void SQLaddShow(char * buffer, int time, int roomID, int movieID){
	answer = buffer;
	type = COUNT_CHECK;
	auxResp = 0;
	sprintf(sql_query, "select count(*) from shows where (shows.roomID = %i and shows.time = %i);", roomID, time);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);	
	if(auxResp){
		sprintf(answer, "Sala no disponible en ese horario");
		return;
	}
	type = COUNT_CHECK;
	auxResp = 0;
	sprintf(sql_query, "select count(*) from rooms where (rooms.roomID = %i);", roomID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);	
	if(!auxResp){
		sprintf(answer, "Sala no existe");
		return;
	}
	type = COUNT_CHECK;
	auxResp = 0;
	sprintf(sql_query, "select count(*) from movies where (movies.movieID = %i);", movieID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);	
	if(!auxResp){
		sprintf(answer, "Pelicula no existe");
		return;
	}
	sprintf(sql_query, "insert into shows (time, movieID, roomID) values (%i, %i, %i);", time, movieID, roomID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	sprintf(answer, "Show agregado");
	return;
}

void SQLremoveShow(char * buffer, int showID){
	answer = buffer;
	sprintf(sql_query, "delete from shows where (showID = %i);", showID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	sprintf(answer, "Show eliminado");
	return;
}

void SQLaddMovie(char * buffer, int length, char * title, char * desc){
	answer = buffer;
	sprintf(sql_query, "insert into movies (length, title, desc) values (%i, %i, %i);", length, title, desc);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	sprintf(answer, "Pelicula agregada");
	return;
}

void SQLremoveMovie(char * buffer, int movieID){
	answer = buffer;
	sprintf(sql_query, "delete from movies where (movieID = %i);", movieID);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);
	sprintf(answer, "Pelicula eliminado");
	return;
}

void SQLundoBuyTicket(char * buffer, int ticketID, char* nombre){
	answer = buffer;
	type = COUNT_CHECK;
	auxResp = 0;
	sprintf(sql_query, "select count(*) from tickets where (ticketID = %i and nombre = '%s');", ticketID, name);
	rc = sqlite3_exec(db, sql_query, callback, (void*) &type, &errMsg);
	if( rc != SQLITE_OK ) printf("error: %s\n", errMsg);	
	if(auxResp){
		sprintf(answer, "Sala no disponible en ese horario");
		return;
	}
}

		
	//creando tablas
void setUpDB(){
	int acc = access( "test.db", F_OK );	
	rc = sqlite3_open("test.db", &db);
	
	if(acc == -1 ){	
		sql_create_rooms = "CREATE TABLE rooms("
			"roomID 	INTEGER	PRIMARY KEY,"
			"maxCap 	int	not null);";
			
		sql_create_movies =	 "create table movies("
			"movieID	integer primary key,"
			"length	int not null,"
			"title		varchar(128) not null,"
			"desc		varchar(1024) not null);";
			
		sql_create_shows =	 "create table shows("
			"showID	integer primary key,"
			"time		int not null,"
			"roomID	int not null,"
			"movieID	int not null," 
			"foreign key (roomID) references rooms(roomID)" 
			"on delete cascade," 
			"foreign key (movieID) references movies(movieID)" 
			"on delete cascade," 
			"unique (roomID, time));";
			
		sql_create_seats =	 "create table seats(" 
			"seatNum	int not null," 
			"roomID		int not null," 
			"primary key (seatNum, roomID)," 
			"foreign key (roomID) references rooms(roomID)" 
			"on delete cascade"
			");" ;
			
		sql_create_tickets =	 "create table tickets(" 
			"ticketID	integer primary key ," 
			"seatNum	int not null," 
			"showID	int not null,"
			"name  	varchar(64),"
			"unique (seatNum, showID)," 
			"foreign key (showID) references shows(showID)" 
			"on delete cascade);";
		
		//////////////////////////
		
		
					
	
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
		
		sql_insert = "insert into movies (movieID, length, title, desc)"
					"values (12, 120, 'volver al futuro', 'vuelven al futuro');";
		rc = sqlite3_exec(db, sql_insert, callback, 0, &errMsg);
		if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
		}
		
		sql_insert = "insert into rooms (maxCap)"
					"values (110),"
					"(110),"
					"(110),"
					"(110);";
		rc = sqlite3_exec(db, sql_insert, callback, 0, &errMsg);
		if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
		}
		
		sql_insert = "insert into shows (time, roomID, movieID)"
					"values (1234, 2, 12),"
					"(2344, 1, 12);";
		rc = sqlite3_exec(db, sql_insert, callback, 0, &errMsg);
		if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
		}
					
		
	//termina de insertar cosas
	}
}