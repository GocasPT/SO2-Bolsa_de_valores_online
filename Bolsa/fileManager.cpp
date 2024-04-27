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
		std::TSTRING name, password;
		float balance;

		//TODO: read file
		while (std::getline(file, line)) {
			ss << line;
			ss >> name >> password >> balance;

			USER user;
			_tcscpy_s(user.name, name.c_str());
			_tcscpy_s(user.password, password.c_str());
			user.balance = balance;
			user.connected = false;

			servidor.userList.push_back(user);
			ss.clear();
		}
		file.close();
	}
	else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficeiro '" << FILE_USERS << "'";
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
		ss << "Erro ao abrir o ficeiro '" << FILE_COMPANIES << "'";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << _T("Ficheiro '") << FILE_COMPANIES << _T("' lido com sucesso (2/") << TOTAL_FILES << _T(")") << std::endl;

	//TODO: maybe add more files to read

	std::_tcout << TAG_NORMAL << _T("Ficheiros lidos com sucesso") << std::endl << std::endl;
}

void Files::write_files(BOLSA& servidor) {
	std::_tcout << _T("A salvar dados no ficheiros... (0/") << TOTAL_FILES << _T(")") << std::endl;

	/*TODO:
	  - get data from servidor.users and write in FILE_USERS
	  - get data from servidor.companies and write in FILE_COMPANIES
	*/

	std::_tcout << TAG_NORMAL << _T("Dados salvos no ficheiroscom sucesso") << std::endl << std::endl;
}