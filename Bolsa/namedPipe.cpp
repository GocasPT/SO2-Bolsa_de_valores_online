#include "namedPipe.h"

DWORD WINAPI clienteReceiver(LPVOID lpParam) {
	//TODO:
	// - cast data
	// - loop reciver and add to list or queue
	// - create thread in case of add list	

	return 0;
}

void sendMessage(BOLSA &bolsa, std::TSTRING msg) {
	//TODO:
	// - format message to send
	// - send message
}