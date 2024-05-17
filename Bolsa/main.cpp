#include "namedPipe.h"
#include "sharedMemory.h"
#include "registry.h"
#include "commands.h"
#include "fileManager.h"
#include "companyManager.h"
#include <fcntl.h>
#include <io.h>

bool checkServerRunning() {
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

void closeServer(BOLSA& servidor) {
	std::_tcout << std::endl << TAG_NORMAL << _T("A fechar o servidor...") << std::endl << std::endl;

	servidor.isRunning = false;

	CompanyManager::close(servidor);
	SharedMemory::close(servidor);
	NamedPipe::close(servidor);
	//TODO: Files::write_files(servidor);
}

void configServer(BOLSA &servidor) {
	std::_tcout << TAG_NORMAL << _T("A configurar o servidor servidor...") << std::endl << std::endl;

	servidor.isRunning = true;
	servidor.isPaused = false;

	try {
		Files::read_files(servidor);
		Registry::config(servidor);
		SharedMemory::config(servidor);
		NamedPipe::config(servidor);
		CompanyManager::config(servidor);
	} catch (std::runtime_error& e) {
		std::_tcout << TAG_ERROR << e.what() << std::endl;
		closeServer(servidor);
	}

	std::_tcout << TAG_NORMAL << _T("Servidor configurado com sucesso") << std::endl << std::endl;
}

int _tmain(int argc, std::TSTRING argv[]) {
	BOLSA servidor;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	if (checkServerRunning()) {
		std::_tcout << TAG_ERROR << _T("Ja existe uma instancia do programa 'Bolsa' em execução") << std::endl;
		exit(-1);
	}

	configServer(servidor);

	cmd::consoleRoutine(servidor);

	closeServer(servidor);

	return 0;
}