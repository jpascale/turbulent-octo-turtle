#include "sharedFunctions.h"
#include "datagram.h"
#include <stdio.h>

void main(){
	connect();

	//SHELL
	printf("Vos quisiste consultar Cartelera\n");
	
	getMovieList();
	printf("Cliente termina\n");
}
