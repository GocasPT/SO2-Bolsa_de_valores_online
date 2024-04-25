#include "SO2_Utils.h"
#include "namedPipe.h"
#include "sharedMemory.h"
#include "bolsa.h"
#include <fcntl.h>
#include <io.h>

BOOL existBolsaRunning() {
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

void setUpServidor(BOLSA& servidor) {
	//TODO: set up servidor
	// - read registry
	// - create shared memory
	// - create named pipe (call center)
	// - create thread (call center)
}

int _tmain(int argc, std::TSTRING argv[]) {
	BOLSA servidor;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	std::tcout << TEXT("Sou o programa \'Bolsa\'") << std::endl;
	std::tcin;

	if (existBolsaRunning()) {
		std::tcout << TEXT("Ja existe uma instancia do programa \'Bolsa\' rodando") << std::endl;
		exit(1);
	}

	setUpServidor(servidor);

	// TODO: main loop (console)

	return 0;
}