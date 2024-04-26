#include "namedPipe.h"

void NamedPipe::config(BOLSA& servidor) {
	HANDLE &hPipe = servidor.hPipe;
	HANDLE &hReciverThread = servidor.hReciverThread;
	CRITICAL_SECTION &cs = servidor.cs;

	//TODO: set input and output for duplex (we can define 2 different structs at will)
	//hPipe = CreateNamedPipe(NAMED_PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(/*INPUT_STRUCT*/), sizeof(/*OUTPUT_STRUCT*/), 1000, NULL);
	hPipe = CreateNamedPipe(NAMED_PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 1000, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) {
		std::tcout << _T("Erro ao criar o named pipe do servidor (") << GetLastError() << _T(")") << std::endl;
		return; //TODO: return error code
	}

	InitializeCriticalSection(&cs);

	servidor.tData.cs = &cs;

	//hReciverThread = CreateThread(NULL, 0, reciverRoutine, &servidor.tData, 0, NULL);
	//if (hReciverThread == NULL) {
	//	std::tcout << _T("Erro ao criar a thread para receber clientes (") << GetLastError() << _T(")") << std::endl;
	//	DeleteCriticalSection(&cs);
	//	CloseHandle(hPipe);
	//	return; //TODO: return error code
	//}
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	/*TODO:
	  - cast data
	  - loop reciver and add to list or queue
	  - create thread in case of add list
	*/

	TDATA *data = (TDATA*)lpParam;

	while (data->isRunning) {
		/*TODO:
		  - wait for client to connect
		  - connection triger, enter critical section
		  - if list is full, add to queue
		  - else, add to list, create thread and create pipe
		  - exit critical section
		*/
	}

	return 0;
}

void NamedPipe::send(BOLSA &servidor, std::TSTRING msg) {
	/*TODO:
	  - format message to send
	  - send message
	*/
}

void NamedPipe::close(BOLSA& servidor) {
	HANDLE &hPipe = servidor.hPipe;
	HANDLE &hReciverThread = servidor.hReciverThread;
	CRITICAL_SECTION &cs = servidor.cs;

	CloseHandle(hReciverThread);
	DeleteCriticalSection(&cs);
	CloseHandle(hPipe);
}