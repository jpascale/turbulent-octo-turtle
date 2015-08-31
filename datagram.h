typedef enum {CONSULTAR_CARTELERA,CONSULTAR_FUNCIONES,CONSULTAR_ASIENTOS
COMPRAR_TICKET,DESHACER_COMPRA} command;


struct Datagram{
	int opcode;
	void* params; // tagUNION
};

typedef enum {
	TO_SERVER, TO_CLIENT
} route;

struct Connection{
	int pid;
	route route;
};

struct Person{
	char * full_name;
	int DNI;
	char * pass;
};

struct Room{
	int roomId;
	int max_cap;
};

struct Movie{
	char * title;
	char * desc;
	int length;
	int movieId;
};

struct Show{
	int showId;
	char * time;
	int roomId;
	int movieId;
};

struct Ticket{
	int ticketId;
	int client_dni;
	int showId;
};