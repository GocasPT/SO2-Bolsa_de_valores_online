#include "SO2_Utils.h"
#include "named pipe.h"
#include "cliente.h"
#include "commands.h"
#include <fcntl.h>
#include <io.h>

bool checkServerIsRunnig() {
	HANDLE hPipeTest = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, PIPE_TIMEOUT, NULL);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hPipeTest);
		return true;
	}
	else {
		CloseHandle(hPipeTest);
		return false;
	}
}

int _tmain(int argc, std::TSTRING argv[]) {
	CLIENTE currentUser;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	std::_tcout << TEXT("Sou o programa \'Cliente\'") << std::endl;
	std::_tcin;

	if (!checkServerIsRunnig()) {
		std::_tcout << TAG_ERROR << TEXT("O servidor não está a correr") << std::endl;
		exit(-1);
	}

	currentUser.logged = false;
	std::_tcout << TAG_NORMAL << TEXT("Executa o comando \'") << CMD_LOGIN << _T("\' para puder ligar ao servidor") << std::endl;

	cmd::consoleRoutine(currentUser);

	//TODO: close all handlers, pipe, threads, etc

	return 0;
}