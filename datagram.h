typedef enum {CONSULTAR_CARTELERA,CONSULTAR_FUNCIONES,CONSULTAR_ASIENTOS
COMPRAR_TICKET,DESHACER_COMPRA} command;


struct Datagram{
	int opcode;
	void* params;
}

struct Connection{
	int pid;
}

