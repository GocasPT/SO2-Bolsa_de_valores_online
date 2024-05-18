#include "namedPipe.h"
#include <sstream>

/**
 * \brief Conectar ao servidor
 * 
 * \param user - Estrutura com os dados do utilizador
 * 
 * \throw std::runtime_error - Erro ao criar o evento para o overlap I/O
 * \throw std::runtime_error - Erro ao esperar pelo servidor
 * \throw std::runtime_error - Erro ao conectar ao servidor
 * \tparam std::runtime_error - Erro ao criar a thread
 */
void NamedPipe::connectToServer(CLIENTE& user) {
	std::_tcout << _T("A preparar o overlap I/O do named pipe... ");

	user.hPipeInst.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (user.hPipeInst.hEvent == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para o overlap I/O (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	ZeroMemory(&user.hPipeInst.oOverlap, sizeof(OVERLAPPED));
	user.hPipeInst.oOverlap.hEvent = user.hPipeInst.hEvent;

	user.hPipeInst.hEventExtra = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (user.hPipeInst.hEventExtra == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para o overlap I/O (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	ZeroMemory(&user.hPipeInst.oOverlapExtra, sizeof(OVERLAPPED));
	user.hPipeInst.oOverlapExtra.hEvent = user.hPipeInst.hEventExtra;

	std::_tcout << _T("Overlap I/O preparado") << std::endl;

	std::_tcout << _T("A conectar ao servidor...");
	if (!WaitNamedPipe(PIPE_BOLSA_NAME, NMPWAIT_WAIT_FOREVER)) {
		std::stringstream ss;

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			ss << "O servidor fechou, a sair do programa...";

		else 
			ss << "Erro ao esperar pelo servidor (" << GetLastError() << ")";

		throw std::runtime_error(ss.str());
	}

	user.hPipeInst.hPipe = CreateFile(PIPE_BOLSA_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED , NULL);
	if (user.hPipeInst.hPipe == NULL) {
		std::stringstream ss;
		ss << "Erro ao conectar ao servidor (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << _T(" Conectado ao servidor") << std::endl;

	user.hThread = CreateThread(NULL, 0, receiverMessage, &user, 0, NULL);
	if (user.hThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << _T("Thread para receber mesagens do servidor criada com sucesso") << std::endl;
}

/**
 * \brief Rotina para receber mensagens do servidor
 * 
 * \param lpParam - Ponteiro para a estrutura com os dados do utilizador
 * 
 * \return 0 - Saiu normalmente
 * \return 1 - Autenticação negada
 * \return -1 - Erro ao ler a mensagem
 * \return 3 - O servidor encerrou
 */
DWORD WINAPI NamedPipe::receiverMessage(LPVOID lpParam) {
	CLIENTE* user = (CLIENTE*)lpParam;
	MESSAGE msg;
	BOOL ret;
	DWORD nBytes;

	std::_tcout << _T("à espera de autenticação... ");

	while (user->runnig) {
		ret = ReadFile(user->hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, &user->hPipeInst.oOverlap);
		while (!ret) {
			switch (GetLastError()) {
				case ERROR_IO_PENDING:
					WaitForSingleObject(user->hPipeInst.hEvent, INFINITE);
					ret = GetOverlappedResult(user->hPipeInst.hPipe, &user->hPipeInst.oOverlap, &nBytes, FALSE);
					break;

				case ERROR_BROKEN_PIPE:
				case ERROR_PIPE_NOT_CONNECTED:
					std::_tcout << std::endl << TAG_ERROR << TEXT("O servidor encerrou. Pressione 'Enter' para sair do programa...") << std::endl;
					user->runnig = false;
					user->logged = false;
					SetEvent(user->hEventConsole);
					return THREAD_CODE::STOP;

				case ERROR_OPERATION_ABORTED:
					SetEvent(user->hEventConsole);
					return THREAD_CODE::SUCESS;

				default:
					std::_tcout << TAG_ERROR << TEXT("Erro ao ler a mensagem (") << GetLastError() << _T(")") << std::endl;
					SetEvent(user->hEventConsole);
					return THREAD_CODE::ERRO;
			}
		}

		switch (msg.code) {
			case CODE_LOGIN:
				if (user->logged)
					break;

				user->logged = true;
				std::_tcout << TEXT("User autenticado. Bem-vindo ") << user->name << _T("!") << std::endl;
				break;

			case CODE_DENID:
				std::_tcout << TEXT("Autenticação negada. Verifique as credênciais") << std::endl;
				SetEvent(user->hEventConsole);
				return THREAD_CODE::STOP;

			case CODE_FULL:
				user->inQueue = true;
				std::_tcout << TEXT("Servidor cheio, está na fila de espera. Tem permissão para utilizar alguns comandos") << std::endl;
				break;

			//TODO: recevie free_slot message (its connected and talking with)
			case CODE_FREE_SLOT:
				user->inQueue = false;
				std::_tcout << TEXT("Já está a ser atendido pelo servidor") << std::endl;
				break;

			case CODE_LISTC_ITEM: {
				if (_tcscmp(msg.data, _T("\0")) == 0)
					break;

				std::TSTRING companyName;
				DWORD numFreeStocks;
				float pricePerStock;
				std::_tstringstream ss;

				ss << msg.data;
				ss >> companyName >> numFreeStocks >> pricePerStock;

				 std::_tcout << _T("Nome: ") << companyName << _T(" | Nº de Ações: ") << numFreeStocks << _T(" | Preço por Ação: ") << pricePerStock << std::endl;

				 continue;
			}

			case CODE_BALANCE:
				std::_tcout << _T("O seu saldo é ") << msg.data << std::endl;
				break;

			case CODE_GENERIC_FEEDBACK:
				std::_tcout << TAG_NORMAL << msg.data << std::endl;
				break;

			case CODE_NOTIFY: {
				std::TSTRING companyName;
				float oldPirce, newPrice;
				std::_tstringstream ss;

				ss << msg.data;
				ss >> companyName >> oldPirce >> newPrice;

				std::_tcout << std::endl << _T("O preço da empresa '") << companyName << _T("' ") << ( (oldPirce < newPrice) ? _T("aumentou") : _T("diminuiu") ) << _T(" de ") << oldPirce << _T(" para ") << newPrice << _T("!") << std::endl;

				break;
			}
		}

		SetEvent(user->hEventConsole);
	}

	return THREAD_CODE::SUCESS;
}

/**
 * \brief Enviar uma mensagem ao servidor
 * 
 * \param user - Estrutura com os dados do utilizador
 * \param msg - Mensagem a enviar
 * 
 * \throw std::runtime_error - Erro ao enviar a mensagem
 */
void NamedPipe::send(CLIENTE& user, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	ret = WriteFile(user.hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, &user.hPipeInst.oOverlapExtra);
	while (!ret) {
		switch (GetLastError()) {
			case ERROR_IO_PENDING:
				WaitForSingleObject(user.hPipeInst.hEventExtra, INFINITE);
				ret = GetOverlappedResult(user.hPipeInst.hPipe, &user.hPipeInst.oOverlapExtra, &nBytes, FALSE);
				break;

			default:
				std::stringstream ss;
				ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
				throw std::runtime_error(ss.str());
		}
	}
}

/**
 * \brief Pedir ao servidor para fazer login
 * 
 * \param user - Estrutura com os dados do utilizador
 * \param name - Nome do utilizador
 * \param password - Password do utilizador
 */
void NamedPipe::requestLogin(CLIENTE& user, std::TSTRING name, std::TSTRING password) {
	try {
		connectToServer(user);

		_tcscpy_s(user.name, name.c_str());

		std::_tstringstream ss;
		ss << name << _T(" ") << password;

		MESSAGE msg;
		msg.code = CODE_LOGIN;
		_tcscpy_s(msg.data, ss.str().c_str());

		send(user, msg);

		while (!user.logged || user.inQueue) {} // Espera pela autenticação e que não esteja na fila para libertar a consola
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

/**
 * \brief Pedir ao servidor para listar as empresas
 * 
 * \param user - Estrutura com os dados do utilizador
 */
void NamedPipe::requestList(CLIENTE& user) {
	try {
		send(user, { CODE_LISTC });
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

/**
 * \brief Pedir ao servidor para comprar ações
 * 
 * \param user - Estrutura com os dados do utilizador
 * \param company - Nome da empresa
 * \param numOfStocks - Número de ações a comprar
 */
void NamedPipe::requestBuy(CLIENTE& user, std::TSTRING company, DWORD numOfStocks) {
	try {
		std::_tstringstream ss;
		ss << company << _T(" ") << numOfStocks;

		MESSAGE msg;
		msg.code = CODE_BUY;
		_tcscpy_s(msg.data, ss.str().c_str());

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

/**
 * \brief Pedir ao servidor para vender ações
 * 
 * \param user - Estrutura com os dados do utilizador
 * \param company - Nome da empresa
 * \param numOfStocks - Número de ações a vender
 */
void NamedPipe::requestSell(CLIENTE& user, std::TSTRING company, DWORD numOfStocks) {
	try {
		std::_tstringstream ss;
		ss << company << _T(" ") << numOfStocks;

		MESSAGE msg;
		msg.code = CODE_SELL;
		_tcscpy_s(msg.data, ss.str().c_str());

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

/**
 * \brief Pedir ao servidor para consultar o saldo
 * 
 * \param user - Estrutura com os dados do utilizador
 */
void NamedPipe::requestBalance(CLIENTE& user) {
	try {
		send(user, { CODE_BALANCE });
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

/**
 * \brief Terminar e fechar tudo que é relacionado com o named pipe
 * 
 * \param user - Estrutura com os dados do utilizador
 */
void NamedPipe::close(CLIENTE& user) {
	CancelIoEx(user.hPipeInst.hPipe, &user.hPipeInst.oOverlap);

	WaitForSingleObject(user.hThread, INFINITE);

	CloseHandle(user.hThread);

	CloseHandle(user.hPipeInst.hPipe);
	CloseHandle(user.hPipeInst.hEvent);
}