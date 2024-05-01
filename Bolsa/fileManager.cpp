#include "fileManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <atlstr.h>

void Files::read_files(BOLSA& servidor) {
	std::_tcout << _T("A ler ficheiros... (0/") << TOTAL_FILES << _T(")") << std::endl;

	std::_tifstream file;
	std::TSTRING line;
	std::_tstringstream ss;

	file.open(FILE_USERS);
	if (file.is_open()) {
		while (std::getline(file, line)) {
			USER user;
			ss << line;
			ss >> user.name >> user.password >> user.balance;
			user.connected = false;
			user.inQueue = false;
			user.hPipeInst = INVALID_HANDLE_VALUE;

			servidor.userList.push_back(user);
			ss.clear();
		}
		file.close();
	}
	else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficeiro 'users.txt'"; //TODO: use FILE_USERS [UNICODE → ANSII]
		throw std::runtime_error(ss.str());
	}
	
	std::_tcout << _T("Ficheiro '") << FILE_USERS << _T("' lido com sucesso (1/") << TOTAL_FILES << _T(")") << std::endl;

	file.open(FILE_COMPANIES);
	if (file.is_open()) {
		std::TSTRING name;
		int numFreeStocks;
		float pricePerStock;

		while (std::getline(file, line)) {
			ss << line;
			ss >> name >> numFreeStocks >> pricePerStock;

			COMPANY company;
			_tcscpy_s(company.name, name.c_str());
			company.numFreeStocks = numFreeStocks;
			company.pricePerStock = pricePerStock;

			servidor.companyList.push_back(company);
			ss.clear();
		}
		file.close();
	}
	else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficeiro 'companies.txt'"; //TODO: use FILE_COMPANIES [UNICODE → ANSII]
		throw std::runtime_error(ss.str());
	}
	std::_tcout << _T("Ficheiro '") << FILE_COMPANIES << _T("' lido com sucesso (2/") << TOTAL_FILES << _T(")") << std::endl;

	//TODO: maybe add more files to read

	std::_tcout << TAG_NORMAL << _T("Ficheiros lidos com sucesso") << std::endl << std::endl;
}

void Files::write_files(BOLSA& servidor) {
	std::_tcout << _T("A salvar dados no ficheiros... (0/") << TOTAL_FILES - 1 << _T(")") << std::endl;

	/*TODO:
	  - get data from servidor.companies and write in FILE_COMPANIES
	  - maybe more files to write/save
	*/

	std::_tcout << TAG_NORMAL << _T("Dados salvos no ficheiroscom sucesso") << std::endl << std::endl;
}