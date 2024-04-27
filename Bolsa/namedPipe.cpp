#include "namedPipe.h"
#include <sstream>

void NamedPipe::config(BOLSA& servidor) {
	std::tcout << _T("A configurar o named pipe para receber os clientes...") << std::endl;

	//TODO: check if the input and output size are correct
	servidor.hPipe = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);
	if (servidor.hPipe == INVALID_HANDLE_VALUE) {
		std::stringstream ss;
		ss << _T("Criação do named pipe do servidor (") << GetLastError() << _T(")");
		throw std::runtime_error(ss.str());
	}

	InitializeCriticalSection(&servidor.cs);

	servidor.tData = { true, servidor.hPipe, &servidor.userList, &servidor.userQueue, 10, &servidor.cs };

	servidor.hReciverThread = CreateThread(NULL, 0, reciverRoutine, &servidor.tData, 0, NULL);
	if (servidor.hReciverThread == NULL) {
		std::stringstream ss;
		ss << _T("Erro ao criar a thread para receber clientes (") << GetLastError() << _T(")");
		throw std::runtime_error(ss.str());
	}

	std::tcout << TAG_NORMAL << _T("Configuração do named piep conculida, já está à esperade um cliente para connectar") << std::endl << std::endl;
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	/*TODO:
	  - cast data
	  - loop reciver and add to list or queue
	  - create thread in case of add list
	*/

	TDATA *data = (TDATA*)lpParam;
	BOOL fConnected = FALSE;

	while (data->isRunning) {
	
		/*TODO:
		  - wait for client to connect
		  - connection triger, enter critical section
		  - if list is full, add to queue
		  - else, add to list, create thread and create pipe
		  - exit critical section
		*/

		fConnected = ConnectNamedPipe(data->hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (!fConnected) {
			std::tcout << TAG_ERROR << _T("Erro ao conectar o cliente ao named pipe do servidor (") << GetLastError() << _T(")") << std::endl;
			data->isRunning = FALSE;
			continue;
		}

		std::tcout << TAG_NORMAL << _T("Cliente conectado ao named pipe do servidor") << std::endl;
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
	std::tcout << _T("A fechar o named pipe do servidor...") << std::endl;

	//TODO: PLACEHOLDER
	// connect to server named pipe to unclock the reciver thread

	for (HANDLE hThread : servidor.hUsersThreadList)
		WaitForSingleObject(hThread, INFINITE); //TODO: maybe change to WaitForMultipleObjects
	WaitForSingleObject(servidor.hReciverThread, INFINITE);

	CloseHandle(servidor.hReciverThread);
	DeleteCriticalSection(&servidor.cs);
	CloseHandle(servidor.hPipe);

	std::tcout << TAG_NORMAL << _T("Named pipe fechado com sucesso") << std::endl << std::endl;
}