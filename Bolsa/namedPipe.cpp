﻿#include "namedPipe.h"
#include "userManager.h"
#include <sstream>

void NamedPipe::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar o named pipe para receber os clientes...") << std::endl;

	InitializeCriticalSection(&servidor.cs);

	servidor.hReciverThread = CreateThread(NULL, 0, reciverRoutine, &servidor, 0, NULL);
	if (servidor.hReciverThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread para receber clientes (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << TAG_NORMAL << _T("Configuração do named piep conculida, já está à esperade um cliente para connectar") << std::endl << std::endl;
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	BOLSA* data = (BOLSA*)lpParam;

	while (data->isRunning) {
	
		/*TODO:
		  x wait for client to connect
		  - connection triger, enter critical section
		  - if list is full, add to queue
		  - else, add to list, create thread and create pipe
		  - exit critical section
		*/

		HANDLE hNewPipe = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);
		if (hNewPipe == INVALID_HANDLE_VALUE) {
			std::_tcout << TAG_ERROR << _T("Erro ao criar o named pipe do servidor (") << GetLastError() << _T(")") << std::endl;
			data->isRunning = false;
			break;
		}

		if (!ConnectNamedPipe(hNewPipe, NULL)) {
			std::_tcout << TAG_ERROR << _T("Erro ao conectar o cliente ao named pipe do servidor (") << GetLastError() << _T(")") << std::endl;
			data->isRunning = false;
			break;
		}

		/*TODO
		  x read named pipe
		  x show message info
		  x validade user
		  - if valid, add to list or queue
		  - if add to list, create thread
		  - else, send message to client (denid code)
		*/

		//TODO: maybe move this to a function (authinticateUser)
		BOOL ret;
		DWORD nBytes;
		MESSAGE msg;
		std::_tstringstream ss;


		ret = ReadFile(hNewPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
		if (!ret || !nBytes) {
			std::_tcout << TAG_ERROR << _T("Erro ao ler a mensagem do cliente (") << GetLastError() << _T(")") << std::endl;
			data->isRunning = false;
			break;
		}

		//TODO:PLACEHOLDER
		std::_tcout << TAG_NORMAL << _T("Mensagem recebida: ") << msg.data << _T(" [CODE: ") << msg.code << _T("]") << std::endl;

		USER loginUser;
		ss << msg.data;
		ss >> loginUser.name >> loginUser.password;

		if (UserManager::validateUser(data->userList, loginUser)) {
			try {
				USER& user = UserManager::getUser(data->userList, loginUser.name);
				user.connected = true;

				send(hNewPipe, { CODE_LOGIN, _T('\0')}); // Manda mensagem de login com sucesso

				std::_tcout << std::endl << TAG_NORMAL << _T("Cliente ") << user.name  << _T(" conectou ao servidor pelo named pipe") << std::endl;

				//TODO: add user to list or queue
				data->hUsersPipesList.push_back(hNewPipe);
				TDATA newTDate = { data->isRunning, hNewPipe, data->userList, &user, data->cs };
				data->tDataList.push_back(newTDate);

				std::_tcout << _T("Criando thread para comunicação com o cliente...") << std::endl; //TODO: show user info
				HANDLE newUserThread = CreateThread(NULL, 0, userRoutine, &newTDate, 0, NULL);
				if (newUserThread == NULL) {
					std::_tcout << TAG_ERROR << _T("Erro ao criar a thread para o cliente (") << GetLastError() << _T(")") << std::endl;
					data->isRunning = false;
					break;
				}
				data->hUsersThreadList.push_back(newUserThread);
				std::_tcout << _T("Thread do cliente criada com sucesso") << std::endl; //TODO: show user info

				std::_tcout << TAG_NORMAL << _T("A criar um named pipe para recber um novo cliente...") << std::endl;
			} catch (std::runtime_error& e) {
				std::_tcout << TAG_ERROR << e.what() << std::endl;
			}
		} else
			send(hNewPipe, { (DWORD) CODE_DENID, _T('\0')}); // Manda mensagem de login inválido
		//TODO: maybe move this to a function (authinticateUser)
	}

	return 0;
}

DWORD WINAPI NamedPipe::userRoutine(LPVOID lpParam) {
	/*TODO
	  x cast lpParam
	  x loop to recive messages
	  - entre critical section, get/set data (copy/modify), leave critical section
	  - send message
	  - close pipe
	*/

	TDATA* data = (TDATA*)lpParam;
	MESSAGE msg;
	BOOL ret;
	DWORD nBytes;
	std::_tstringstream ss;

	while (data->isRunning) {
		/*TODO
		  x read message
		  x show message info
		  - enter critical section
		  - get/set data
		  - leave critical section
		  - send message
		*/
		//TODO: Cliente exit → close pipe + thread + other data

		ret = ReadFile(data->hPipe, (LPVOID) &msg, sizeof(MESSAGE), &nBytes, NULL);
		if (!ret || !nBytes) {
			if (GetLastError() == ERROR_BROKEN_PIPE) {
				std::_tcout << std::endl << TAG_NORMAL << _T("Cliente ") << data->myUser->name << _T(" desconectado") << std::endl; //TODO: show user info, close thread, set user offline
			} else {
				std::_tcout << std::endl << TAG_ERROR << _T("Erro ao ler a mensagem do cliente (") << GetLastError() << _T(")") << std::endl;
			}

			data->isRunning = false;
			break;
		}

		std::_tcout << std::endl << TAG_NORMAL << _T("Mensagem recebida: ") << msg.data << _T(" [CODE: ") << msg.code << _T("]") << std::endl;

		EnterCriticalSection(&data->cs);

		if (msg.code == CODE_LISTC) {
			//TODO: loop to send all companies (item by item)
			send(data->hPipe, msg);
		}

		LeaveCriticalSection(&data->cs);
	}

	CloseHandle(data->hPipe);

	return 0;
}

void NamedPipe::send(HANDLE hPipe, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	ret = WriteFile(hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		std::stringstream ss;
		ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << TAG_NORMAL << _T("Mensagem enviada: ") << msg.data << _T(" [CODE: ") << msg.code << _T("]") << std::endl;
}

//TODO [DEBUG]: something is throwing an exception
void NamedPipe::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar o named pipe do servidor...") << std::endl;

	//TODO: connect to server named pipe to unclock the reciver thread

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