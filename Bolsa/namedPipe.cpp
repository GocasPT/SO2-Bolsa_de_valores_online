#include "namedPipe.h"

void NamedPipe::config(BOLSA& servidor) {
	/*TODO:
	  - create named pipe
	  - set up handler
	  - set thread to reciver
	  - set up critical section
	*/
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	//TODO:
	// - cast data
	// - loop reciver and add to list or queue
	// - create thread in case of add list	

	return 0;
}

void NamedPipe::send(BOLSA &servidor, std::TSTRING msg) {
	//TODO:
	// - format message to send
	// - send message
}