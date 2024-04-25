#include "namedPipe.h"
#include "sharedMemory.h"
#include "registry.h"
#include <fcntl.h>
#include <io.h>

BOOL checkServerRunning() {
	HANDLE hPipeTest = CreateNamedPipe(NAMED_PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);
	if (GetLastError() == ERROR_PIPE_BUSY) {
		CloseHandle(hPipeTest);
		return TRUE;
	}
	else {
		CloseHandle(hPipeTest);
		return FALSE;
	}
}

void configServer(BOLSA &servidor) {
	Registry::config(servidor);
	SharedMemory::config(servidor);
	NamedPipe::config(servidor);
}

void consoleRoutine(BOLSA& servidor) {
	std::TSTRING input;
	std::vector<std::TSTRING> args;

	//TODO: change this for flag (any error, break loop → for safe) 
	do {
		std::tcout << _T(">> ");
		std::getline(std::tcin, input);

		//TODO: slipt input into args → args[0] = cmd, args[1] = arg1, args[2] = arg2, etc

		//TODO: PLACEHOLDER
		std::tcout << _T("Comando: \'") << input << _T("\'") << std::endl;

	} while (input.compare(_T("close")));
}

void closeServer(BOLSA& servidor) {
	/*TODO:
	  - CloseHandle for all handles
	  - etc
	*/
}

int _tmain(int argc, std::TSTRING argv[]) {
	BOLSA servidor;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	std::tcout << TEXT("Sou o programa \'Bolsa\'") << std::endl;
	std::tcin;

	if (checkServerRunning()) {
		std::tcout << TEXT("Ja existe uma instancia do programa \'Bolsa\' rodando") << std::endl;
		exit(1);
	}

	configServer(servidor);

	consoleRoutine(servidor);

	closeServer(servidor);

	return 0;
}