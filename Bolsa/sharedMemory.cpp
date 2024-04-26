#include "sharedMemory.h"

void SharedMemory::config(BOLSA& servidor) {
	std::tcout << _T("A configurar a memória partilhada para mandar aos board(GUI)s...") << std::endl;

	/*TODO:
	  - create shared memory
	  - set up handler
	  - set up event
	*/

	std::tcout << TAG_NORMAL << _T("Configuração da memória partillhada conculida, disponível para os programas board(GUI)s conectarem-se") << std::endl << std::endl;
}

void SharedMemory::update(BOLSA &servidor) {
	/*TODO
	  - format date to write
	  - write data
	  - trigger event
	*/
}

void SharedMemory::close(BOLSA& servidor) {
	std::tcout << _T("A fechar a memória partilhada...") << std::endl;

	/*TODO:
	  - close handler
	  - close event
	*/

	std::tcout << TAG_NORMAL << _T("Memória partilhada fechado com sucesso") << std::endl << std::endl;
}