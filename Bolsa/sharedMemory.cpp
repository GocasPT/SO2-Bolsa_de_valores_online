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
		std::_tcout << _T("[ERRO] Falha na criação da memória partilhada do tabuleiro do jogo") << GetLastError() << std::endl;
		return;
	}

	servidor.data = reinterpret_cast<SHARED_MEMORY*>(MapViewOfFile(servidor.hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SHARED_MEMORY)));
	if (servidor.data == NULL) {
		std::_tcout << _T("[ERRO] Falha na associação da memória partilhada do tabuleiro do jogo") << GetLastError() << std::endl;
		close(servidor);
		return;
	}

	std::_tcout << TAG_NORMAL << _T("Configuração da memória partillhada conculida, disponível para os programas board(GUI)s conectarem-se") << std::endl << std::endl;
}

void SharedMemory::update(BOLSA &servidor) {
	/*TODO
	  - format date to write
	  - write data
	  - trigger event
	*/
}

void SharedMemory::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar a memória partilhada...") << std::endl;

	/*TODO:
	  - close handler
	  - close event
	*/

	std::_tcout << TAG_NORMAL << _T("Memória partilhada fechado com sucesso") << std::endl << std::endl;
}