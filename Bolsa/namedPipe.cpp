#include "namedPipe.h"
#include <sstream>

void NamedPipe::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar o named pipe para receber os clientes...") << std::endl;

	//TODO: check if the input and output size are correct
	servidor.hPipe = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);
	if (servidor.hPipe == INVALID_HANDLE_VALUE) {
		std::stringstream ss;
		ss << "Criacao do named pipe do servidor (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	InitializeCriticalSection(&servidor.cs);

	servidor.tData = { true, servidor.hPipe, &servidor.userList, &servidor.userQueue, &servidor.hUsersThreadList, &servidor.hUsersPipesList, 10, &servidor.cs }; //TODO: change '10' to correct value

	servidor.hReciverThread = CreateThread(NULL, 0, reciverRoutine, &servidor.tData, 0, NULL);
	if (servidor.hReciverThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread para receber clientes (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << TAG_NORMAL << _T("Configuração do named piep conculida, já está à esperade um cliente para connectar") << std::endl << std::endl;
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	/*TODO:
	  - cast data
	  - loop reciver and add to list or queue
	  - create thread in case of add list
	*/

	TDATA *data = (TDATA*)lpParam;
	bool fConnected = FALSE;

	while (data->isRunning) {
	
		/*TODO:
		  - wait for client to connect
		  - connection triger, enter critical section
		  - if list is full, add to queue
		  - else, add to list, create thread and create pipe
		  - exit critical section
		*/

		fConnected = ConnectNamedPipe(data->hPipe, NULL) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (!fConnected) {
			std::_tcout << TAG_ERROR << _T("Erro ao conectar o cliente ao named pipe do servidor (") << GetLastError() << _T(")") << std::endl;
			data->isRunning = FALSE;
			continue;
		}

		std::_tcout << std::endl << TAG_NORMAL << _T("Cliente conectado ao named pipe do servidor") << std::endl;

		/*TODO
		  - creathe thread to handle client
		*/
		data->hUsersPipesList->push_back(data->hPipe);

		std::_tcout << _T("Criando thread para comunicação com o cliente...") << std::endl;
		HANDLE newUserThread = CreateThread(NULL, 0, userRoutine, data->hPipe, 0, NULL); //TODO: check if the param is correct
		if (newUserThread == NULL) {
			std::_tcout << TAG_ERROR << _T("Erro ao criar a thread para o cliente (") << GetLastError() << _T(")") << std::endl;
			data->isRunning = FALSE;
			continue;
		}

		data->hUsersThreadList->push_back(newUserThread);
		
		std::_tcout << _T("Criando novo pipe para receber novo cliente...") << std::endl;
		data->hPipe = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);
	}

	return 0;
}

DWORD WINAPI NamedPipe::userRoutine(LPVOID lpParam) {
	/*TODO
	  - cast lpParam
	  - loop to recive messages
	  - entre critical section, get/set data (copy/modify), leave critical section
	  - send message
	  - close pipe [?]
	*/

	std::_tcout << _T("Thread do cliente criada com sucesso") << std::endl;

	return 0;
}

void NamedPipe::send(BOLSA &servidor, std::TSTRING msg) {
	/*TODO:
	  - format message to send
	  - send message
	*/
}

void NamedPipe::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar o named pipe do servidor...") << std::endl;

	//TODO: PLACEHOLDER
	// connect to server named pipe to unclock the reciver thread

	for (HANDLE hThread : servidor.hUsersThreadList)
		WaitForSingleObject(hThread, INFINITE); //TODO: maybe change to WaitForMultipleObjects
	WaitForSingleObject(servidor.hReciverThread, INFINITE);

	//TODO: maybe show user info in same time
	for (DWORD i = 0; i < servidor.hUsersPipesList.size(); i++) {
		std::_tcout << _T("A fecher o pipe do cliente ") << i << _T(" de ") << servidor.hUsersPipesList.size() << std::endl;
		if (!DisconnectNamedPipe(servidor.hUsersPipesList[i])) {
			std::_tcout << TAG_ERROR << _T("Erro ao fechar o pipe do cliente ") << i << _T(" (") << GetLastError() << _T(")") << std::endl;
		}

		CloseHandle(servidor.hUsersThreadList[i]);
		CloseHandle(servidor.hUsersPipesList[i]);
	}

	CloseHandle(servidor.hReciverThread);
	DeleteCriticalSection(&servidor.cs);
	CloseHandle(servidor.hPipe);

	std::_tcout << TAG_NORMAL << _T("Named pipe fechado com sucesso") << std::endl << std::endl;
}