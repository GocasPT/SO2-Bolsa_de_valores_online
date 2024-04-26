#include "companyManager.h"
#include <sstream>

//TODO: check
void CompanyManager::addCompany(BOLSA& servidor, std::TSTRING name, std::TSTRING numOfStock, std::TSTRING pricePerStock) {
	if (getCompany(servidor, name) != nullptr) {
		std::tcout << TAG_WARNING << _T("Empresa já existe") << std::endl;
		return;
	}

	std::wstringstream ss; //TODO: change to macro (tstringstream → wstringstream or stringstream)
	ss << numOfStock << " " << pricePerStock;

	DWORD numStocks;
	float priceStock;
	ss >> numStocks >> priceStock;

	COMPANY company;
	_tcscpy_s(company.name, name.c_str());
	company.numFreeStocks = numStocks;
	company.pricePerStock = priceStock;

	servidor.companyList.push_back(company);

	std::tcout << TAG_NORMAL << _T("Empresa adicionada com sucesso") << std::endl <<
		_T("Nome: ") << name << _T(" | Nº de Ações: ") << numStocks << _T(" | Preço por Ação: ") << priceStock << std::endl << std::endl;
}

void CompanyManager::listCompanies(BOLSA& servidor) {
	std::tcout << TAG_NORMAL << _T("Listagem de Empresas:") << std::endl;
	
	if (servidor.companyList.empty()) {
		std::tcout << _T("Nenhuma empresa") << std::endl;
		return;
	}

	for(auto it = servidor.companyList.begin(); it != servidor.companyList.end(); it++)
		std::tcout << _T("Nome: ") << it->name << _T(" | Nº de Ações: ") << it->numFreeStocks << _T(" | Preço por Ação: ") << it->pricePerStock << std::endl;

	std::tcout << std::endl;
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