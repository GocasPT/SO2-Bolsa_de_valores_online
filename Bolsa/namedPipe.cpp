﻿#include "namedPipe.h"
#include "userManager.h"
#include "companyManager.h"
#include "stockWalletManager.h"
#include <sstream>

HANDLE NamedPipe::newNamedPipe() {
	//TODO: add overlapped IO
	return CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(MESSAGE), sizeof(MESSAGE), PIPE_TIMEOUT, NULL);
}

void NamedPipe::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar o named pipe para receber os clientes...") << std::endl;

	servidor.hPipeInst = newNamedPipe();
	if (servidor.hPipeInst == INVALID_HANDLE_VALUE) {
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

	std::_tcout << TAG_NORMAL << _T("Configuração do named piep concluida, já está à esperade um cliente para connectar") << std::endl << std::endl;
}

DWORD WINAPI NamedPipe::reciverRoutine(LPVOID lpParam) {
	BOLSA* data = (BOLSA*)lpParam;
	USER loginUser;

	std::_tcout << _T("A espera de um cliente para conectar...") << std::endl;

	try {
		while (data->isRunning) {
			if (!ConnectNamedPipe(data->hPipeInst, NULL)) {
				if (GetLastError() == ERROR_BROKEN_PIPE)
					break;

				std::stringstream ss;
				ss << "Erro ao conectar o cliente ao named pipe do servidor (" << GetLastError() << ")";
				throw std::runtime_error(ss.str());
			}

			EnterCriticalSection(&data->cs);

			if (auth(*data, loginUser)) {
				USER& user = UserManager::getUser(data->userList, loginUser.name);
				user.hPipeInst = data->hPipeInst;

				if (UserManager::addUser(*data, &user)) {
					user.connected = true;
					TDATA newTDate = { data->isRunning, data->isPaused, data->userList, data->hUsersQueue, data->companyList, &user, data->cs };
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
					send(data->hPipeInst, { CODE_FULL, _T('\0') });
					user.connected = false; //TODO: check this
				}
				
			} else {
				CloseHandle(data->hPipeInst);
			}

			std::_tcout << _T("A criar um novo named pipe para recber um novo cliente...") << std::endl;
			data->hPipeInst = newNamedPipe();
			if (data->hPipeInst == INVALID_HANDLE_VALUE) {
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

	std::_tcout << _T("A sair do reciverRoutine...") << std::endl;

	return 0;
}

bool NamedPipe::auth(BOLSA& servidor, USER& loginUser) {
	BOOL ret;
	DWORD nBytes;
	MESSAGE msg;

	// Lê a mensagem de login do cliente
	ret = ReadFile(servidor.hPipeInst, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		if (GetLastError() == ERROR_BROKEN_PIPE)
			return false;

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

	send(servidor.hPipeInst, { code, _T('\0') }); // Manda mensagem de login

	return code == CODE_LOGIN; // Retorna se o login foi válido
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

			while (data->isRunning) {
				ret = ReadFile(data->myUser->hPipeInst, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
				if (!ret || !nBytes) {
					DWORD error = GetLastError();
					if (error == ERROR_BROKEN_PIPE || error == ERROR_PIPE_NOT_CONNECTED)
						std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Cliente ") << data->myUser->name << _T(" desconectado") << std::endl;
					else
						std::_tcout << std::endl << TAG_ERROR _T("[THREAD ") << tID << _T("] Erro ao ler a mensagem do cliente (") << GetLastError() << _T(")") << std::endl;

					break;
				}

				std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Mensagem recebida: ") << msg.data << _T(" [CODE: ") << msg.code << _T("]") << std::endl;

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
				}

				//TODO: improve this (when enter in critical secntion? When recive message OR get/set data?)
				LeaveCriticalSection(&data->cs);

				ss.clear();
				ss.flush();
				ss.str(std::TSTRING());
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
	std::_tcout << std::endl << _T("[THREAD ") << tID << _T("] Não existe nenhum cliente para atener. A sair...") << std::endl;

	return 0;
}

void NamedPipe::send(HANDLE hPipeInst, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	ret = WriteFile(hPipeInst, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		std::stringstream ss;
		ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
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
			//TODO: update late user op + trigger event to update shared memory
		} else
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
		//TODO: update late user op + trigger event to update shared memory
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

//TODO [DEBUG]: something is throwing an exception
void NamedPipe::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar o named pipe do servidor...") << std::endl;

	for (DWORD i = 0; i < servidor.hUsersList.size(); i++) {
		std::_tcout << _T("A fecher o pipe do cliente ") << i << _T(" de ") << servidor.hUsersList.size();
		if (!DisconnectNamedPipe(servidor.hUsersList[i])) {
			std::_tcout << std::endl << TAG_ERROR << _T("Erro ao fechar o pipe do cliente ") << i << _T(" (") << GetLastError() << _T(")") << std::endl;
		}

		CloseHandle(servidor.hUsersThreadList[i]);
		CloseHandle(servidor.hUsersList[i]);

		std::_tcout << _T(" | Fechado") << std::endl;
	}

	WaitForMultipleObjects(static_cast<DWORD>(servidor.hUsersThreadList.size()), servidor.hUsersThreadList.data(), TRUE, INFINITE);

	CloseHandle(servidor.hPipeInst);

	WaitForSingleObject(servidor.hReciverThread, INFINITE);
	CloseHandle(servidor.hReciverThread);

	DeleteCriticalSection(&servidor.cs);

	std::_tcout << TAG_NORMAL << _T("Named pipe fechado com sucesso") << std::endl << std::endl;
}