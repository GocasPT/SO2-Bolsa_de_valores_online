#include "namedPipe.h"
#include "sharedMemory.h"
#include "registry.h"
#include "commands.h"
#include "fileManager.h"
#include <fcntl.h>
#include <io.h>

BOOL checkServerRunning() {
	HANDLE hPipeTest = CreateNamedPipe(PIPE_BOLSA_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, PIPE_TIMEOUT, NULL);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(hPipeTest);
		return TRUE;
	}
	else {
		CloseHandle(hPipeTest);
		return FALSE;
	}
}

void configServer(BOLSA &servidor) {
	std::tcout << TAG_NORMAL << _T("A inicial servidor...") << std::endl << std::endl;

	Files::read_files(servidor);
	Registry::config(servidor);
	SharedMemory::config(servidor);
	NamedPipe::config(servidor);

	//TODO: check if all handlers are not NULL (error code)
}

void closeServer(BOLSA& servidor) {
	std::tcout << std::endl <<
		TAG_NORMAL << _T("A fechar o servidor...") << std::endl << std::endl;

	servidor.tData.isRunning = FALSE;

	WaitForMultipleObjects(1, &servidor.hReciverThread, TRUE, INFINITE); //TODO: check if is this argument is correct

	SharedMemory::close(servidor);
	NamedPipe::close(servidor);
	Files::write_files(servidor);
}

int _tmain(int argc, std::TSTRING argv[]) {
	BOLSA servidor;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	if (checkServerRunning()) {
		std::tcout << TAG_ERROR << _T("Ja existe uma instancia do programa \'Bolsa\' em execução") << std::endl;
		exit(1);
	}

	configServer(servidor);

	cmd::consoleRoutine(servidor);

	closeServer(servidor);

	return 0;
}