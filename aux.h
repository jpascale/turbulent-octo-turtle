sendData(Connection * connection, int size, void * params);
receiveData(Connection * sender, int size, void * buffer);




Data * data = malloc(sizeof(Data));
Connection * sender = malloc(sizeof(Connection));

while(1) {
	receiveData(sender, sizeof(Data), data);
	ProcessData(sender, data);	
}

ProcessData(sender, data)
	Buy(int cliente, bla, bla);
		BuyResponse(cliente, response)
			sendData(clientMap[cliente], response, sizeof(BuyResponseData));
cliente
