#include "sharedMemory.h"

void SharedMemory::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar a memória partilhada para mandar aos board(GUI)s...") << std::endl;

	/*TODO:
	  - create shared memory
	  - set up handler
	  - set up event
	*/

	servidor.hSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SHARED_MEMORY), SHARED_MEMORY_NAME);
	if (servidor.hSharedMemory == NULL) {
		std::_tcout << _T("[ERRO] Falha na criação da memória partilhada dos dados") << GetLastError() << std::endl;
		return;
	}

	servidor.sharedMemory = reinterpret_cast<SHARED_MEMORY*>(MapViewOfFile(servidor.hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SHARED_MEMORY)));
	if (servidor.sharedMemory == NULL) {
		std::_tcout << _T("[ERRO] Falha na associação da memória partilhada dos dados") << GetLastError() << std::endl;
		close(servidor);
		return;
	}

	servidor.hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
	if (servidor.hEvent == NULL) {
		std::_tcout << _T("[ERRO] Falha na criação do evento") << GetLastError() << std::endl;
		close(servidor);
		return;
	}

	std::_tcout << TAG_NORMAL << _T("Configuração da memória partillhada concluída, disponível para os programas board(GUI)s se conectarem") << std::endl << std::endl;
}

void SharedMemory::update(BOLSA &servidor) {
	ResetEvent(servidor.hEvent);

	ZeroMemory(servidor.sharedMemory->companies, sizeof(COMPANY) * N);
	std::copy(servidor.companyList.begin(), servidor.companyList.end(), servidor.sharedMemory->companies);

	// TODO acrescentar última transação: servidor.data->lastTransaction = servidor.lastTransaction;

	/*TODO
	  - format date to write
	  - write data
	  - trigger event
	*/


	SetEvent(servidor.hEvent);
}

void SharedMemory::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar a memória partilhada...") << std::endl;

	CloseHandle(servidor.hEvent);

	UnmapViewOfFile(servidor.sharedMemory);
	CloseHandle(servidor.hSharedMemory);

	std::_tcout << TAG_NORMAL << _T("Memória partilhada fechada com sucesso") << std::endl << std::endl;
}