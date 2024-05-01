#include "namedPipe.h"
#include <sstream>

void NamedPipe::connectToServer(CLIENTE& user) {
	std::_tcout << _T("A preparar o overlap I/O do named pipe... ");
	user.hPipeInst.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (user.hPipeInst.hEvent == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para o overlap I/O (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	ZeroMemory(&user.hPipeInst.overlap, sizeof(OVERLAPPED));
	user.hPipeInst.overlap.hEvent = user.hPipeInst.hEvent;
	std::_tcout << _T("Overlap I/O preparado") << std::endl;

	std::_tcout << _T("A conectar ao servidor...");
	if (!WaitNamedPipe(PIPE_BOLSA_NAME, NMPWAIT_WAIT_FOREVER)) {
		std::stringstream ss;
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

	user.hThread = CreateThread(NULL, 0, reciverMessage, &user, 0, NULL);
	if (user.hThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << _T("Thread para recber mesagens do servidor criada com sucesso") << std::endl;
}

DWORD WINAPI NamedPipe::reciverMessage(LPVOID lpParam) {
	CLIENTE* user = (CLIENTE*)lpParam;
	MESSAGE msg;
	BOOL ret;
	DWORD nBytes;

	std::_tcout << _T("A espera de autenticação... ");

	while (user->runnig) {
		ret = ReadFile(user->hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, &user->hPipeInst.overlap);
		if (!ret || !nBytes) {
			if (GetLastError() == ERROR_BROKEN_PIPE) {
				std::_tcout << TAG_NORMAL << TEXT("O servidor encerrou. A sair do programa...") << std::endl;
				user->runnig = false;
				user->logged = false;
				return 3;
			}

			else if (GetLastError() == ERROR_IO_PENDING) {
				WaitForSingleObject(user->hPipeInst.hEvent, INFINITE);
			}

			else {
				std::_tcout << TAG_ERROR << TEXT("Erro ao ler a mensagem") << std::endl;
				return -1;
			}
		}

		// Se não tiver feito login, ignora todas as mensagens exceto o login
		if (!user->logged && msg.code != CODE_LOGIN)
			continue;

		switch (msg.code) {
			case CODE_LOGIN:
				user->logged = true;
				std::_tcout << TEXT("User autnenticado. Bem-vindo ") << user->name << _T("!") << std::endl;
				break;

			case CODE_DENID:
				std::_tcout << TEXT("Autenticação negada. Verifique as credência novamente") << std::endl;
				return 1;

			case CODE_FULL:
				user->inQueue = true;
				std::_tcout << TEXT("Servidor cheio, estás na fila de espera. Tens permissão para utilizar alguns comandos") << std::endl;
				break;

			//TODO: recevie free_slot message (its connected and talking with)

			case CODE_LISTC_ITEM:
				//TODO: Show item from list companies (table mode)
				std::_tcout << std::endl << msg.data;
				break;

			case CODE_BALANCE:
				std::_tcout << _T("O seu saldo é ") << msg.data << std::endl;
				break;

			case CODE_GENERIC_FEEDBACK:
				std::_tcout << TAG_NORMAL << msg.data << std::endl; //TODO: change tag (server tag or feedback tag)
				break;
		}
	}

	return 0;
}

void NamedPipe::send(CLIENTE& user, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	//TODO: Add overlapped I/O maybe
	ret = WriteFile(user.hPipeInst.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		std::stringstream ss;
		ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
}

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

void NamedPipe::requestList(CLIENTE& user) {
	try {
		send(user, { CODE_LISTC , _T('\0') });
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

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

void NamedPipe::requestBalance(CLIENTE& user) {
	try {
		send(user, { CODE_BALANCE, _T('\0')});
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

void NamedPipe::close(CLIENTE& user) {
	CloseHandle(user.hPipeInst.hPipe);
	CloseHandle(user.hPipeInst.hEvent);
}