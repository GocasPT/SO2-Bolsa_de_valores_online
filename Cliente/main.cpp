#include "SO2_Utils.h"
#include "namedPipe.h"
#include "cliente.h"
#include "commands.h"
#include <fcntl.h>
#include <io.h>

bool checkServerIsRunnig() {
	HANDLE hPipeTest = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, PIPE_TIMEOUT, NULL);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hPipeTest);
		return true;
	}
	else {
		CloseHandle(hPipeTest);
		return false;
	}
}

void configServer(CLIENTE& user) {
	user.hEventConsole = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (user.hEventConsole == NULL) {
		std::_tcout << TAG_ERROR << TEXT("Erro ao criar o evento para a consola") << std::endl;
		exit(-1);
	}

	user.runnig = true;
	user.logged = false;
	user.inQueue = false;
}

int _tmain(int argc, std::TSTRING argv[]) {
	CLIENTE currentUser;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	if (!checkServerIsRunnig()) {
		std::_tcout << TAG_ERROR << TEXT("O servidor não está a correr") << std::endl;
		exit(-1);
	}

	configServer(currentUser);	

	std::_tcout << TAG_NORMAL << TEXT("Executa o comando \'") << CMD_LOGIN << _T("\' para puder ligar ao servidor") << std::endl;

	cmd::consoleRoutine(currentUser);

	std::_tcout << std::endl << TAG_NORMAL << _T("A sair do programa cliente...") << std::endl << std::endl;

	NamedPipe::close(currentUser);

	return 0;
}