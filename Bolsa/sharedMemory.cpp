#include "sharedMemory.h"

void SharedMemory::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar a mem�ria partilhada para mandar aos board(GUI)s...") << std::endl;



	std::_tcout << TAG_NORMAL << _T("Configura��o da mem�ria partillhada conculida, dispon�vel para os programas board(GUI)s conectarem-se") << std::endl << std::endl;
}

void SharedMemory::update(BOLSA &servidor) {

}

void SharedMemory::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar a mem�ria partilhada...") << std::endl;



	std::_tcout << TAG_NORMAL << _T("Mem�ria partilhada fechado com sucesso") << std::endl << std::endl;
}