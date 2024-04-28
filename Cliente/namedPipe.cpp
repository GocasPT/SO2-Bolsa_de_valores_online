#include "namedPipe.h"
#include <sstream>

void NamedPipe::connectToServer(CLIENTE& user) {
	/*TODO:
	  - Connect to server
	  - Create thread to receive messages
	*/

	std::_tcout << TEXT("A conectar ao servidor...") << std::endl;
	if (!WaitNamedPipe(PIPE_BOLSA_NAME, NMPWAIT_WAIT_FOREVER)) {
		std::stringstream ss;
		ss << "Erro ao esperar pelo servidor (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	user.hPipe = CreateFile(PIPE_BOLSA_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (user.hPipe == INVALID_HANDLE_VALUE) {
	if (user.hPipe == NULL) {
		std::stringstream ss;
		ss << "Erro ao conectar ao servidor (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << TEXT("Conectado ao servidor") << std::endl;

	user.hThread = CreateThread(NULL, 0, reciverMessage, &user, 0, NULL); //TODO: Check parameter pointer
	if (user.hThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << TAG_NORMAL << TEXT("Thread criada com sucesso") << std::endl;
}

DWORD WINAPI NamedPipe::reciverMessage(LPVOID lpParam) {
	/*TODO:
	  - Cast lpParam → struct
	  - Receive message from server
	  - Show message to user
	*/

	CLIENTE* user = (CLIENTE*)lpParam; //TODO: Change to user struct maybe
	MESSAGE msg;
	BOOL ret;
	DWORD nBytes;

	while (!user->tContinue) {
		//TODO: On disconnect, close/exit program
		ret = ReadFile(user->hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
		if (!ret || !nBytes) {
			std::_tcout << TAG_ERROR << TEXT("Erro ao ler a mensagem") << std::endl;
			return -1;
		}
		else {
			std::_tcout << TAG_NORMAL << TEXT("Mensagem recebida: ") << msg.data << std::endl;
		}
	}

	return 0;
}

void NamedPipe::send(CLIENTE& user, MESSAGE msg) {
	BOOL ret;
	DWORD nBytes;

	ret = WriteFile(user.hPipe, (LPVOID)&msg, sizeof(MESSAGE), &nBytes, NULL);
	if (!ret || !nBytes) {
		std::stringstream ss;
		ss << "Erro ao enviar a mensagem [ " << ret << " " << nBytes << "] ()" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}
}

void NamedPipe::requestLogin(CLIENTE& user, std::TSTRING name, std::TSTRING password) {
	/*TODO
	  - Request login to server
	*/

	try {
		connectToServer(user);

		std::TSTRING data = name + _T(" ") + password;

		MESSAGE msg;
		msg.code = CODE_LOGIN;
		_tcscpy_s(msg.data, data.c_str());

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

void NamedPipe::requestList(CLIENTE& user) {
	/*TODO
	  - Request list to server
	*/

	try {
		MESSAGE msg;
		msg.code = CODE_LISTC;

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

void NamedPipe::requestBuy(CLIENTE& user, std::TSTRING company, DWORD numOfStocks) {
	/*TODO
	  - Request buy to server
	*/

	try {
		std::TSTRING data = company + _T(" ") + std::to_wstring(numOfStocks);

		MESSAGE msg;
		msg.code = CODE_BUY;
		_tcscpy_s(msg.data, data.c_str());

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

void NamedPipe::requestSell(CLIENTE& user, std::TSTRING company, DWORD numOfStocks) {
	/*TODO
	  - Request sell to server
	*/

	try {
		std::TSTRING data = company + _T(" ") + std::to_wstring(numOfStocks);

		MESSAGE msg;
		msg.code = CODE_SELL;
		_tcscpy_s(msg.data, data.c_str());

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

void NamedPipe::requestBalance(CLIENTE& user) {
	/*TODO
	  - Request balance to server
	*/

	try {
		MESSAGE msg;
		msg.code = CODE_BALANCE;

		send(user, msg);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
	}
}

void NamedPipe::close(CLIENTE& user) {
	/*TODO
	  - Close pipe
	  - Terminate thread
	  - etc
	*/
}