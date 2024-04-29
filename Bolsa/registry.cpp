#include "registry.h"

void Registry::config(BOLSA& servidor) {
	std::_tcout << _T("A buscar os dados ao Registry...") << std::endl;

	/*TODO: read data from registry
	  - if not exist, create registry with default value and set it
	*/

	servidor.maxUsers = 1; //TODO: change '10' to correct value

	std::_tcout << TAG_NORMAL << _T("Dados lidos com sucesso") << std::endl << std::endl;
}

void Registry::create() {
	/*TODO:
	  - create registry
	  - create key
	  - set value with default value
	*/

	std::_tcout << _T("Criado o dado default [") << 10 << _T("] no registry") << std::endl << std::endl;
}