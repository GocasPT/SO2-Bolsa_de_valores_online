#include "namedPipe.h"
#include "userManager.h"
#include "companyManager.h"
#include "stockWalletManager.h"
#include <sstream>

PIPE_INST NamedPipe::newNamedPipe() {
	PIPE_INST newPipeInst;

	newPipeInst.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (newPipeInst.hEvent == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para a nova instancia de named pipe (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	ZeroMemory(&newPipeInst.oOverlap, sizeof(newPipeInst.oOverlap));
	newPipeInst.oOverlap.hEvent = newPipeInst.hEvent;

	newPipeInst.hPipe = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);
	if (newPipeInst.hPipe == INVALID_HANDLE_VALUE) {
		std::stringstream ss;
		ss << "Erro ao criar nova instancia de named pipe (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	return newPipeInst;
}

void NamedPipe::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar o named pipe para receber os clientes...") << std::endl;

	/*---NAMED PIPE INICIAL---*/
	servidor.hPipeInst = newNamedPipe();

	/*---CRITICAL SECTION---*/
	InitializeCriticalSection(&servidor.cs);

	/*---THREAD PARA GERIR LISTAS---*/
	servidor.hDataEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (servidor.hDataEvent == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para gerir as listas (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	servidor.hDataThread = CreateThread(NULL, 0, dataRoutine, &servidor, 0, NULL);
	if (servidor.hDataThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread para gerir as listas (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	/*---THREAD PARA MANDAR NOTIFICAÇÕES---*/
	servidor.notifyData.company = nullptr;
	servidor.notifyData.oldPrice = 0;

	servidor.hNotifyEvent = CreateEvent(NULL, FALSE, FALSE, EVENT_NOTIFY);
	if (servidor.hNotifyEvent == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para mandar notificações (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	servidor.hNotifyThread = CreateThread(NULL, 0, notifyRoutine, &servidor, 0, NULL);
	if (servidor.hNotifyThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread para mandar notificações (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	/*---THREAD PARA RECEBER CLIENTES---*/
	servidor.hReciverThread = CreateThread(NULL, 0, reciverRoutine, &servidor, 0, NULL);
	if (servidor.hReciverThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread para receber clientes (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << TAG_NORMAL << _T("Configuração do named piep concluida, já está à esperade um cliente para connectar") << std::endl << std::endl;
}

bool NamedPipe::auth(BOLSA& servidor, USER& loginUser) {
	BOOL ret;
	DWORD nBytes;
	MESSAGE msg;

	// Lê a mensagem de login do cliente
	ret = ReadFile(servidor.hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, &servidor.hPipeInst.oOverlap);
	while (!ret) {
		switch (GetLastError()) {
			case ERROR_IO_PENDING:
				WaitForSingleObject(servidor.hPipeInst.hEvent, INFINITE);
				ret = GetOverlappedResult(servidor.hPipeInst.hPipe, &servidor.hPipeInst.oOverlap, &nBytes, FALSE);
				break;

			case ERROR_BROKEN_PIPE:
				return false;

			default:
				std::stringstream ss;
				ss << "Erro ao ler a mensagem do cliente (" << GetLastError() << ")";
				throw std::runtime_error(ss.str());
		}
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

	send(servidor.hPipeInst, { code, _T('\0') }); // Manda mensagem de login

	return code == CODE_LOGIN; // Retorna se o login foi válido
}

DWORD WINAPI NamedPipe::dataRoutine(LPVOID lpParam) {
	BOLSA* data = (BOLSA*)lpParam;

	//TODO: data manager logic (basiclly remove close handles)
	/*TODO
	  x wait event
	  x enter CS
	  - re-organize data
	  x leave CS
	*/

	while (data->isRunning) {
		WaitForSingleObject(data->hDataEvent, INFINITE);

		if (!data->isRunning)
			break;

		EnterCriticalSection(&data->cs);

		//TODO: data manager logic
		//TODO: lick TData with Thread (better select)

		LeaveCriticalSection(&data->cs);
	}

	return 0;
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	BOLSA* data = (BOLSA*)lpParam;
	USER loginUser;
	BOOL ret;

	std::_tcout << TAG_NORMAL << _T("A espera de um cliente para conectar...") << std::endl;

	try {
		while (data->isRunning) {
			ret = ConnectNamedPipe(data->hPipeInst.hPipe, &data->hPipeInst.oOverlap);
			while (!ret) {
				switch (GetLastError()) {
					case ERROR_IO_PENDING:
						WaitForSingleObject(data->hPipeInst.hEvent, INFINITE);
						ret = TRUE;
						break;

					case ERROR_BROKEN_PIPE:
					case ERROR_PIPE_CONNECTED:
						std::_tcout << std::endl << TAG_ERROR << _T("O cliente já está conectado") << std::endl;
						return 1;

					default:
						std::stringstream ss;
						ss << "Erro ao conectar o cliente ao named pipe do servidor (" << GetLastError() << ")";
						throw std::runtime_error(ss.str());
				}
			}

			EnterCriticalSection(&data->cs); //TODO: move this down

			if (auth(*data, loginUser)) {
				USER& user = UserManager::getUser(data->userList, loginUser.name);
				user.hPipeInst = data->hPipeInst;

				std::_tcout << std::endl << _T("Cliente ") << user.name;

				if (UserManager::addUser(*data, &user)) {
					std::_tcout << _T(" conectado") << std::endl;

					user.connected = true;
					TDATA newTDate = { data->isRunning, data->isPaused, // Referencias
						data->userList, data->hUsersQueue, data->companyList, // Listas
						data->notifyData, &user, data->cs // Dados + CS
					};
					data->tDataList.push_back(newTDate);

					std::_tcout << _T("A criar thread para comunicação com cliente...") << std::endl;
					HANDLE newUserThread = CreateThread(NULL, 0, userRoutine, &data->tDataList.back(), 0, NULL);
					if (newUserThread == NULL) {
						std::stringstream ss;
						ss << "Erro ao criar a thread para o cliente (" << GetLastError() << ")";
						throw std::runtime_error(ss.str());
					}
					data->hUsersThreadList.push_back(newUserThread);
				} else {
					std::_tcout << _T("na fila de espera") << std::endl;

					user.connected = false;
					send(data->hPipeInst, { CODE_FULL, _T('\0') });
				}
				
			} else {
				//TODO: maybe close other things
				CloseHandle(data->hPipeInst.hPipe);
			}

			std::_tcout << _T("A criar um novo named pipe para recber um novo cliente...");
			data->hPipeInst = newNamedPipe();
			std::_tcout << _T(" Sucesso") << std::endl;

			LeaveCriticalSection(&data->cs); //TODO: move this up

			std::_tcout << TAG_INPUT;
		}
	} catch (std::runtime_error& e) {
		std::_tcout << std::endl << TAG_ERROR << e.what() << std::endl;
		LeaveCriticalSection(&data->cs);
		data->isRunning = false;
	}

	std::_tcout << _T("A sair do reciverRoutine...") << std::endl;

	return 0;
}

DWORD WINAPI NamedPipe::notifyRoutine(LPVOID lpParam) {
	BOLSA* data = (BOLSA*)lpParam;
	std::_tstringstream ss;
	NOTIFY_DATA notify;
	MESSAGE msg;
	msg.code = CODE_NOTIFY;

	//TODO: notify system
	/*TODO
	  x wait for event
	  x send all users the message
	*/

	while (data->isRunning) {
		WaitForSingleObject(data->hNotifyEvent, INFINITE);

		if (!data->isRunning)
			break;

		//TODO: format message
		//TODO: mutex OR cs
		notify = data->notifyData;

		ss.str(std::TSTRING());

		ss <<_T("O preço da empresa ") << notify.company->name << _T(" foi alterado de ") << notify.oldPrice << _T(" para ") << notify.company->pricePerStock;
		_tcscpy_s(msg.data, ss.str().c_str());

		std::_tcout << TAG_NORMAL << ss.str();

		sendAll(data->userList, msg);

		std::_tcout << TAG_INPUT;
	}

	return 0;
}

DWORD WINAPI NamedPipe::userRoutine(LPVOID lpParam) {
	TDATA* data = (TDATA*)lpParam;
	static DWORD tIDCount = 1;
	MESSAGE msg;
	BOOL ret;
	DWORD nBytes, tID;
	std::_tstringstream ss;

	tID = tIDCount++;
	std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Iniciada, pronto para receber pedidos");

	try {
		do {
			std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] A comunicar com o cliente ") << data->myUser->name << std::endl;

			//TODO: send message to client (its connected and talking with)
			// send(data->myUser->hPipeInst, { CODE_FULL, _T('\0') });

			while (data->isRunning && data->myUser->connected) {
				ret = ReadFile(data->myUser->hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, &data->myUser->hPipeInst.oOverlap);
				while (!ret) {
					switch (GetLastError()) {
						case ERROR_IO_PENDING:
							WaitForSingleObject(data->myUser->hPipeInst.hEvent, INFINITE);
							ret = GetOverlappedResult(data->myUser->hPipeInst.hPipe, &data->myUser->hPipeInst.oOverlap, &nBytes, TRUE);
							break;

						case ERROR_BROKEN_PIPE:
						case ERROR_PIPE_NOT_CONNECTED:
							std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Cliente ") << data->myUser->name << _T(" desconectado") << std::endl;
							data->myUser->connected = false;
							ret = TRUE;
							break;

						default:
							std::_tcout << std::endl << TAG_ERROR _T("[THREAD ") << tID << _T("] Erro ao ler a mensagem do cliente (") << GetLastError() << _T(")") << std::endl;
							break;
					}
				}

				if (!data->myUser->connected)
					break;

				//TODO: DEBUG
				std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Mensagem recebida: ") << msg.data << _T(" [CODE: ") << msg.code << _T("] (") << data->myUser->name << _T(")") << std::endl;

				//TODO: improve this (when enter in critical secntion? When recive message OR get/set data?)
				EnterCriticalSection(&data->cs);

				switch (msg.code) {
					case CODE_LISTC: 
						responseList(*data);
						break;
						
					case CODE_BUY: {
						ss.str(msg.data);

						std::TSTRING companyName;
						ss >> companyName;

						DWORD amount;
						ss >> amount;

						responseBuy(*data, companyName, amount);
						break;
					} // !CODE_BUY
						
					case CODE_SELL: {
						ss.str(msg.data);

						std::TSTRING companyName;
						ss >> companyName;

						DWORD amount;
						ss >> amount;

						responseSell(*data, companyName, amount);
						break;
					} // !CODE_SELL

					case CODE_BALANCE:
						responseBalance(*data);
						break;

					case CODE_EXIT:
						std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Cliente ") << data->myUser->name << _T(" desconectado") << std::endl;
						data->myUser->connected = false;
						break;
				}

				//TODO: improve this (when enter in critical secntion? When recive message OR get/set data?)
				LeaveCriticalSection(&data->cs);

				ss.clear();
				ss.flush();
				ss.str(std::TSTRING());

				std::_tcout << TAG_INPUT;
			}

			EnterCriticalSection(&data->cs);
			data->myUser = UserManager::removeUser(data->userList, data->userQueue, data->myUser);
			LeaveCriticalSection(&data->cs);

		} while (data->myUser != nullptr && data->isRunning);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}

	//TODO: delete this tData from list
	//TODO: thread/resource manager (thread exit, remove from list)
	std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Não existe nenhum cliente para atener. A sair...") << std::endl << TAG_INPUT;

	return 0;
}

void NamedPipe::send(PIPE_INST hPipeInst, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	ret = WriteFile(hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, &hPipeInst.oOverlap);
	while (!ret) {
		switch (GetLastError()) {
			case ERROR_IO_PENDING:
				WaitForSingleObject(hPipeInst.hEvent, INFINITE);
				ret = GetOverlappedResult(hPipeInst.hPipe, &hPipeInst.oOverlap, &nBytes, FALSE);
				break;

			default:
				std::stringstream ss;
				ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
				throw std::runtime_error(ss.str());
		}
	}

	//TODO: DEBUG
	std::_tcout << _T("Mensagem enviada: ") << msg.data << _T(" [CODE: ") << msg.code << _T("]") << std::endl;
}

void NamedPipe::sendAll(USER_LIST userList, MESSAGE msg) {
	//TODO: send message to all users (connected + queue?)
	for (auto &user : userList) {
		if (user.connected)
			send(user.hPipeInst, msg);
	}
}

void NamedPipe::responseList(TDATA& data) {
	MESSAGE msg;
	std::_tstringstream ss;

	if (data.companyList.empty()) {
		send(data.myUser->hPipeInst, { CODE_LISTC_ITEM, _T("Nenhuma empresa disponível") });
	} else {
		for (auto it = data.companyList.begin(); it != data.companyList.end(); it++) {
			ss.str(std::TSTRING());

			ss << _T("Nome: ") << it->name << _T(" | Nº de Ações: ") << it->numFreeStocks << _T(" | Preço por Ação: ") << it->pricePerStock;

			msg.code = CODE_LISTC_ITEM;
			_tcscpy_s(msg.data, ss.str().c_str());

			send(data.myUser->hPipeInst, msg);

			ss.clear();
			ss.flush();
		}
	}

	send(data.myUser->hPipeInst, { CODE_LISTC_ITEM, _T('\0') });
}

void NamedPipe::responseBuy(TDATA& data, std::TSTRING companyName, DWORD amount) {
	if (data.isPaused) {
		send(data.myUser->hPipeInst, { CODE_GENERIC_FEEDBACK, _T("Compra/Venda de ações suspensa") });
		return;
	}

	MESSAGE msg;

	float balance = data.myUser->balance;
	COMPANY* company = CompanyManager::getCompany(data.companyList, companyName);

	if (company == nullptr)
		_tcscpy_s(msg.data, _T("Empresa não encontrada"));

	else if (balance < company->pricePerStock * amount)
		_tcscpy_s(msg.data, _T("Saldo insuficiênte"));

	else if (company->numFreeStocks < amount)
		_tcscpy_s(msg.data, _T("Ações insuficientes"));

	else {
		if (SWManager::addStock(*data.myUser, *company, amount)) {
			_tcscpy_s(msg.data, _T("Compra efetuada com sucesso"));
			CompanyManager::updateStock(data.notifyData, *company, CompanyManager::BUY);
		}
		else
			_tcscpy_s(msg.data, _T("Chegates ao limites de stocks que podes ter"));
	}

	msg.code = CODE_GENERIC_FEEDBACK;
	send(data.myUser->hPipeInst, msg);
}

void NamedPipe::responseSell(TDATA& data, std::TSTRING companyName, DWORD amount) {
	if (data.isPaused) {
		send(data.myUser->hPipeInst, { CODE_GENERIC_FEEDBACK, _T("Compra/Venda de ações suspensa") });
		return;
	}

	MESSAGE msg;

	COMPANY* company = CompanyManager::getCompany(data.companyList, companyName);

	if (company == nullptr)
		_tcscpy_s(msg.data, _T("Empresa não encontrada"));

	if (!SWManager::userHaveStock(*data.myUser, *company))
		_tcscpy_s(msg.data, _T("Não tens ações desta empresa"));

	else if (SWManager::removeStock(*data.myUser, *company, amount)) {
		_tcscpy_s(msg.data, _T("Venda efetuada com sucesso"));
		CompanyManager::updateStock(data.notifyData, *company, CompanyManager::SELL);
	} else
		_tcscpy_s(msg.data, _T("Não tens esse número de ações para vender"));

	msg.code = CODE_GENERIC_FEEDBACK;
	send(data.myUser->hPipeInst, msg);

}

void NamedPipe::responseBalance(TDATA& data) {
	MESSAGE msg;
	std::_tstringstream ss;

	ss << data.myUser->balance;

	msg.code = CODE_BALANCE;
	ss >> msg.data;

	send(data.myUser->hPipeInst, msg);
}

void NamedPipe::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar o named pipe do servidor...") << std::endl;

	for (auto& user : servidor.userList) {
		if (user.connected || user.inQueue) {
			std::_tcout << _T("A fechar o named pipe do cliente ") << user.name << _T("...");

			user.connected = false;
			CancelIoEx(user.hPipeInst.hPipe, &user.hPipeInst.oOverlap);

			if (!DisconnectNamedPipe(user.hPipeInst.hPipe))
				std::_tcout << std::endl << TAG_ERROR << _T("Erro ao fechar o pipe do cliente ") << user.name << _T(" (") << GetLastError() << _T(")") << std::endl;


			CloseHandle(user.hPipeInst.hPipe);
			CloseHandle(user.hPipeInst.hEvent);

			std::_tcout << _T(" Sucesso") << std::endl;
		}
	}

	WaitForMultipleObjects((DWORD)servidor.hUsersThreadList.size(), servidor.hUsersThreadList.data(), TRUE, INFINITE);

	CancelIoEx(servidor.hPipeInst.hPipe, &servidor.hPipeInst.oOverlap);
	
	WaitForSingleObject(servidor.hReciverThread, INFINITE); //TODO: error
	CloseHandle(servidor.hReciverThread);
	CloseHandle(servidor.hPipeInst.hPipe);
	CloseHandle(servidor.hPipeInst.hEvent);

	SetEvent(servidor.hNotifyEvent);
	WaitForSingleObject(servidor.hNotifyThread, INFINITE);
	CloseHandle(servidor.hNotifyThread);
	CloseHandle(servidor.hNotifyEvent);

	SetEvent(servidor.hDataEvent);
	WaitForSingleObject(servidor.hDataThread, INFINITE);
	CloseHandle(servidor.hDataThread);
	CloseHandle(servidor.hDataEvent);

	DeleteCriticalSection(&servidor.cs);

	std::_tcout << _T("Named pipe fechado com sucesso") << std::endl << std::endl;
}