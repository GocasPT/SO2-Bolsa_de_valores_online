#include "named pipe.h"

void NamedPipe::connectToServer(CLIENTE& user) {
	/*TODO:
	  - Connect to server
	  - Create thread to receive messages
	*/

	user.hPipe = CreateFile(PIPE_BOLSA_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (user.hPipe == INVALID_HANDLE_VALUE) {
		std::tcout << TAG_ERROR << TEXT("Erro ao conectar ao servidor") << std::endl;
		exit(-1); //TODO: Change to exception
	}

	else if (GetLastError() == ERROR_PIPE_BUSY) {
		std::tcout << TAG_ERROR << TEXT("Servidor ocupado") << std::endl;
		exit(-1); //TODO: Change to exception
	}

	else {
		std::tcout << TAG_NORMAL << TEXT("Conectado ao servidor") << std::endl;
		user.pipeMode = PIPE_READMODE_MESSAGE;
		if (!SetNamedPipeHandleState(user.hPipe, &user.pipeMode, NULL, NULL)) {
			std::tcout << TAG_ERROR << TEXT("Erro ao configurar o modo do named pipe") << std::endl;
			exit(-1); //TODO: Change to exception
		}
		else {
			std::tcout << TAG_NORMAL << TEXT("Modo configurado") << std::endl;
		}
	}

	user.hThread = CreateThread(NULL, 0, reciverMessage, &user, 0, NULL); //TODO: Check parameter pointer
	if (user.hThread == NULL) {
		std::tcout << TAG_ERROR << TEXT("Erro ao criar a thread") << std::endl;
		exit(-1); //TODO: Change to exception
	}
	else {
		std::tcout << TAG_NORMAL << TEXT("Thread criada") << std::endl;
	}
}

DWORD WINAPI NamedPipe::reciverMessage(LPVOID lpParam) {
	/*TODO:
	  - Cast lpParam → struct
	  - Receive message from server
	  - Show message to user
	*/

	CLIENTE* user = (CLIENTE*)lpParam; //TODO: Change to user struct maybe

	while (!user->tContinue) {
		DWORD dwRead;
		MESSAGE msg;

		if (!ReadFile(user->hPipe, &msg, sizeof(MESSAGE) * sizeof(TCHAR), &dwRead, NULL)) {
			std::tcout << TAG_ERROR << TEXT("Erro ao ler a mensagem") << std::endl;
			exit(-1); //TODO: Change to exception
		}
		else {
			std::tcout << TAG_NORMAL << TEXT("Mensagem recebida: ") << msg.data << std::endl;
		}
	
	}
}

void NamedPipe::send(CLIENTE& user, std::TSTRING msg) {
	/*TODO
	  - Send message to server
	*/

	DWORD dwWrite;
	MESSAGE message;
	_tcscpy_s(message.data, msg.c_str());

	if (!WriteFile(user.hPipe, &message, sizeof(MESSAGE) * sizeof(TCHAR), &dwWrite, NULL)) {
		std::tcout << TAG_ERROR << TEXT("Erro ao enviar a mensagem") << std::endl;
		exit(-1); //TODO: Change to exception
	}
	else {
		std::tcout << TAG_NORMAL << TEXT("Mensagem enviada") << std::endl;
	}
}