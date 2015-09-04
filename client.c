//#include "sharedFunctions.h"
#include "datagram.h"
#include <stdio.h>

void main(){
	//connect();

	Datagram datag;
	datag.size = 1;
	datag.opcode = 2;
	datag.client_pid = 3;

	datag.data.i = 4;

	strcpy(datag.data.m.title, "hola que tal");
	strcpy(datag.data.m.desc, "sadsdasda");
	datag.data.m.length = 5;
	datag.data.m.movieId = 6;



	printf("%i\n",sizeof(datag));
	printf("%c\n", ((char*)&datag)[12]);
	
	void * datag2 = malloc(2068);

	memcpy(datag2, &datag, 2068);

	Datagram * datag3 = (Datagram * ) datag2;
	printf("%c\n", ((char*)datag3)[12]);
	


	//SHELL
//	printf("Vos quisiste consultar Cartelera\n");
	
	//getMovieList();
//	printf("Cliente termina\n");
}
