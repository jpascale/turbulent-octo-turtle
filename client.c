#include "sharedFunctions.h"
#include <stdio.h>

void main(){
	connect(0);

	//SHELL
	printf("Vos quisiste consultar Cartelera\n");
	
	getMovieList();
	printf("Cliente termina\n");
}
