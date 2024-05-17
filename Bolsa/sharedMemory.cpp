#include "sharedMemory.h"

void SharedMemory::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar a memória partilhada para mandar aos board(GUI)s...") << std::endl;



	std::_tcout << TAG_NORMAL << _T("Configuração da memória partillhada conculida, disponível para os programas board(GUI)s conectarem-se") << std::endl << std::endl;
}

void SharedMemory::update(BOLSA &servidor) {

}

void SharedMemory::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar a memória partilhada...") << std::endl;



	std::_tcout << TAG_NORMAL << _T("Memória partilhada fechado com sucesso") << std::endl << std::endl;
}