#include "companyManager.h"
#include <sstream>

//TODO: check
void CompanyManager::addCompany(BOLSA& servidor, std::TSTRING name, std::TSTRING numOfStock, std::TSTRING pricePerStock) {
	if (getCompany(servidor, name) != nullptr) {
		std::_tcout << TAG_WARNING << _T("Empresa já existe") << std::endl;
		return;
	}

	std::_tstringstream ss; //TODO: change to macro (tstringstream → wstringstream or stringstream)
	ss << numOfStock << " " << pricePerStock;
	COMPANY company;
	ss >> company.numFreeStocks >> company.pricePerStock;

	servidor.companyList.push_back(company);

	std::_tcout << TAG_NORMAL << _T("Empresa adicionada com sucesso") << std::endl <<
		_T("Nome: ") << name << _T(" | Nº de Ações: ") << company.numFreeStocks << _T(" | Preço por Ação: ") << company.pricePerStock << std::endl << std::endl;
}

void CompanyManager::listCompanies(BOLSA& servidor) {
	std::_tcout << TAG_NORMAL << _T("Listagem de Empresas:") << std::endl;
	
	if (servidor.companyList.empty()) {
		std::_tcout << _T("Nenhuma empresa") << std::endl;
		return;
	}

	for(auto it = servidor.companyList.begin(); it != servidor.companyList.end(); it++)
		std::_tcout << _T("Nome: ") << it->name << _T(" | Nº de Ações: ") << it->numFreeStocks << _T(" | Preço por Ação: ") << it->pricePerStock << std::endl;

	std::_tcout << std::endl;
}

COMPANY* CompanyManager::getCompany(BOLSA& servidor, std::TSTRING name) {
	/*TODO
	  - search for company
	  - return company
	*/

	return nullptr;
}

void CompanyManager::updateStock(BOLSA& servidor, std::TSTRING name, std::TSTRING pricePerStock) {
	/*TODO
	  - search for company
	  - update stock price
	*/
}