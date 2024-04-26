#include "sharedMemory.h"

void SharedMemory::config(BOLSA& servidor) {
	std::tcout << _T("A configurar a mem�ria partilhada para mandar aos board(GUI)s...") << std::endl;

	/*TODO:
	  - create shared memory
	  - set up handler
	  - set up event
	*/

	std::tcout << TAG_NORMAL << _T("Configura��o da mem�ria partillhada conculida, dispon�vel para os programas board(GUI)s conectarem-se") << std::endl << std::endl;
}

void SharedMemory::update(BOLSA &servidor) {
	/*TODO
	  - format date to write
	  - write data
	  - trigger event
	*/
}

void SharedMemory::close(BOLSA& servidor) {
	std::tcout << _T("A fechar a mem�ria partilhada...") << std::endl;

	/*TODO:
	  - close handler
	  - close event
	*/

	std::tcout << TAG_NORMAL << _T("Mem�ria partilhada fechado com sucesso") << std::endl << std::endl;
}