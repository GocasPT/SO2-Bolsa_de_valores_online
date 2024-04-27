#include "registry.h"

void Registry::config(BOLSA& servidor) {
	std::tcout << _T("A buscar os dados ao Registry...") << std::endl;

	/*TODO: read data from registry
	  - if not exist, create registry with default value and set it
	*/

	servidor.tData = { TRUE, servidor.hPipe, &servidor.userList, &servidor.userQueue, 10 }; //TODO: change '10' to the read value or default value

	std::tcout << TAG_NORMAL << _T("Dados lidos com sucesso") << std::endl << std::endl;
}

void Registry::create() {
	/*TODO:
	  - create registry
	  - create key
	  - set value with default value
	*/

	std::tcout << _T("Criado o dado default [") << 10 << _T("] no registry") << std::endl << std::endl;
}