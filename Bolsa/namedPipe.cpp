#include "namedPipe.h"
#include "userManager.h"
#include <sstream>

HANDLE NamedPipe::newNamedPipe() {
	//TODO: add overlapped IO
	return CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);;
}

void NamedPipe::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar o named pipe para receber os clientes...") << std::endl;

	servidor.hReciverPipe = newNamedPipe();
	if (servidor.hReciverPipe == INVALID_HANDLE_VALUE) {
		std::stringstream ss;
		ss << "Erro ao criar o named pipe do servidor (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

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
	USER loginUser;

	try {
		while (data->isRunning) {
			if (!ConnectNamedPipe(data->hReciverPipe, NULL)) {
				std::stringstream ss;
				ss << "Erro ao conectar o cliente ao named pipe do servidor (" << GetLastError() << ")";
				throw std::runtime_error(ss.str());
			}

			EnterCriticalSection(&data->cs);

			if (auth(*data, loginUser)) {
				USER& user = UserManager::getUser(data->userList, loginUser.name);
				user.hReciverPipe = data->hReciverPipe;

				if (UserManager::addUser(*data, &user)) {
					user.connected = true;
					TDATA newTDate = { data->isRunning, data->userList, data->hUsersQueue, &user, data->cs };
					data->tDataList.push_back(newTDate);

					std::_tcout << _T("Criando thread para comunicação com cliente...") << std::endl;
					HANDLE newUserThread = CreateThread(NULL, 0, userRoutine, &newTDate, 0, NULL);
					if (newUserThread == NULL) {
						std::stringstream ss;
						ss << "Erro ao criar a thread para o cliente (" << GetLastError() << ")";
						throw std::runtime_error(ss.str());
					}
					data->hUsersThreadList.push_back(newUserThread);
					std::_tcout << _T("Thread de comunicação com cliente criada com sucesso") << std::endl;
				} else {
					//TODO: check if need to do something more
					send(data->hReciverPipe, { CODE_FULL, _T('\0') });
					user.connected = false;
				}
				
			} else {
				CloseHandle(data->hReciverPipe);
			}

			std::_tcout << _T("A criar um novo named pipe para recber um novo cliente...") << std::endl;
			data->hReciverPipe = newNamedPipe();
			if (data->hReciverPipe == INVALID_HANDLE_VALUE) {
				std::stringstream ss;
				ss << "Erro ao criar o named pipe do servidor (" << GetLastError() << ")";
				throw std::runtime_error(ss.str());
			}

			LeaveCriticalSection(&data->cs);
		}
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
		LeaveCriticalSection(&data->cs);
		data->isRunning = false;
	}

	return 0;
}

bool NamedPipe::auth(BOLSA& servidor, USER& loginUser) {
	BOOL ret;
	DWORD nBytes;
	MESSAGE msg;

	// Lê a mensagem de login do cliente
	ret = ReadFile(servidor.hReciverPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		std::stringstream ss;
		ss << "Erro ao ler a mensagem do cliente (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
	
	// Extrai os dados da mensagem
	std::_tstringstream ss;
	ss << msg.data;
	ss >> loginUser.name >> loginUser.password;
	DWORD code;

	if (UserManager::validateUser(servidor.userList, loginUser))
		code = CODE_LOGIN; // Login válido
	else
		code = CODE_DENID; // Login inválido

	send(servidor.hReciverPipe, { code, _T('\0') }); // Manda mensagem de login

	return code == CODE_LOGIN; // Retorna se o login foi válido
}

DWORD WINAPI NamedPipe::userRoutine(LPVOID lpParam) {
	// BASE THREAD ROUTINE (recive messages, send messages, close pipe)
	/*TODO
	  x read message
	  x show message info
	  - enter critical section
	  - get/set data
	  - leave critical section
	  - send message
	*/

	// USER MANAGER THREAD ROUTINE (leave cliente, enter new clientefrom queue if existe)
	/*TODO
	  - cliente exit
	  - close pipe
	  - check if queue have user
	  - if have, get user from queue, add to list, use this thread and use the pipe
	  - else, close thread
	*/

	TDATA* data = (TDATA*)lpParam;
	MESSAGE msg;
	BOOL ret;
	DWORD nBytes, tID;
	std::_tstringstream ss;

	tID = GetCurrentThreadId();
	std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Iniciada, pronto para receber pedidos");

	//TODO: <START> LOOP QUEUE CLIENTE
	do {
		std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] A comunicar com o cliente ") << data->myUser->name << std::endl;

		while (data->isRunning) {
			ret = ReadFile(data->myUser->hReciverPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
			if (!ret || !nBytes) {
				if (GetLastError() == ERROR_BROKEN_PIPE)
					std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Cliente ") << data->myUser->name << _T(" desconectado") << std::endl;
				else
					std::_tcout << std::endl << TAG_ERROR _T("[THREAD ") << tID << _T("] Erro ao ler a mensagem do cliente (") << GetLastError() << _T(")") << std::endl;

				break;
			}

			std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Mensagem recebida: ") << msg.data << _T(" [CODE: ") << msg.code << _T("]") << std::endl;

			//TODO: improve this
			EnterCriticalSection(&data->cs);

			switch (msg.code) {
				case CODE_LISTC:
					//TODO: loop to send all companies (item by item)
					//send(data->myUser.hReciverPipe, msg);

					//while (auto company : data->)

					break;

				case CODE_BUY:
					/*TODO
					  - check user balance, company, amount, price, amount to buy
					  - update user stock wallet, balance and data
					  - send feedback to user
					*/
					break;

				case CODE_SELL:
					/*TODO
					  - check user stock wallet
					  - update user stock wallet, balance and data
					  - send feedback to user
					*/
					break;

				case CODE_BALANCE:
					/*TODO
					  - send user balance
					*/
					break;
			}

			LeaveCriticalSection(&data->cs);
		}

		try {
			data->myUser = UserManager::removeUser(data->userList, data->userQueue, data->myUser);
		} catch (std::runtime_error& e) {
			std::_tcout << TAG_ERROR << e.what() << std::endl;
		}
		

	} while (data->myUser != nullptr && data->isRunning);

	//TODO: delete this tData from list
	std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Não existe nenhum cliente para atener. A sair...") << std::endl;

	return 0;
}

void NamedPipe::send(HANDLE hReciverPipe, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	ret = WriteFile(hReciverPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		std::stringstream ss;
		ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
}

//TODO [DEBUG]: something is throwing an exception
void NamedPipe::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar o named pipe do servidor...") << std::endl;

	//TODO: connect to server named pipe to unclock the reciver thread

	for (HANDLE hThread : servidor.hUsersThreadList)
		WaitForSingleObject(hThread, INFINITE); //TODO: maybe change to WaitForMultipleObjects
	WaitForSingleObject(servidor.hReciverThread, INFINITE);

	//TODO: maybe show user info in same time
	for (DWORD i = 0; i < servidor.hUsersList.size(); i++) {
		std::_tcout << _T("A fecher o pipe do cliente ") << i << _T(" de ") << servidor.hUsersList.size() << std::endl;
		if (!DisconnectNamedPipe(servidor.hUsersList[i])) {
			std::_tcout << TAG_ERROR << _T("Erro ao fechar o pipe do cliente ") << i << _T(" (") << GetLastError() << _T(")") << std::endl;
		}

		CloseHandle(servidor.hUsersThreadList[i]);
		CloseHandle(servidor.hUsersList[i]);
	}

	CloseHandle(servidor.hReciverThread);
	DeleteCriticalSection(&servidor.cs);
	CloseHandle(servidor.hReciverPipe);

	std::_tcout << TAG_NORMAL << _T("Named pipe fechado com sucesso") << std::endl << std::endl;
}