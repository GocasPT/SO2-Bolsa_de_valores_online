#include "fileManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
//#include <atlstr.h>

void Files::read_files(BOLSA& servidor) {
	std::_tcout << _T("A ler ficheiros... (0/") << TOTAL_FILES << _T(")") << std::endl;

	std::_tifstream file;
	std::TSTRING line;
	std::_tstringstream ss;

	file.open(FILE_USERS);
	if (file.is_open()) {
		while (std::getline(file, line)) {
			USER user{};
			ss << line;
			ss >> user.name >> user.password >> user.balance;
			user.connected = false;
			user.inQueue = false;
			user.hPipeInst.hPipe = INVALID_HANDLE_VALUE;
			user.walletSize = 0;

			servidor.userList.push_back(user);
			ss.clear();
		}
		file.close();

	} else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficheiro '" << FILE_USERS << "'";
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

			COMPANY company{};
			_tcscpy_s(company.name, name.c_str());
			company.numFreeStocks = numFreeStocks;
			company.pricePerStock = pricePerStock;

			servidor.companyList.push_back(company);
			ss.clear();
		}
		file.close();

	} else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficheiro '" << FILE_COMPANIES << "'";
		throw std::runtime_error(ss.str());
	}
	std::_tcout << _T("Ficheiro '") << FILE_COMPANIES << _T("' lido com sucesso (2/") << TOTAL_FILES << _T(")") << std::endl;

	std::_tcout << _T("Ficheiros lidos com sucesso") << std::endl << std::endl;
}

void Files::write_files(BOLSA& servidor) {
	std::_tcout << _T("A guardar os dados nos ficheiros... (0/") << TOTAL_FILES - 1 << _T(")") << std::endl;

	std::_tofstream file;

	file.open(FILE_USERS, std::ios::out | std::ios::trunc);
	if (file.is_open()) {
		for (const USER &user : servidor.userList)
			file << user.name << _T(" ") << user.password << _T(" ") << user.balance << std::endl;
		
		file.close();

	} else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficheiro '" << FILE_USERS << "'";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << _T("Dados guardados no ficheiro '") << FILE_USERS << _T("' com sucesso (1/") << TOTAL_FILES - 1 << _T(")") << std::endl;

	file.open(FILE_COMPANIES, std::ios::out | std::ios::trunc);
	if (file.is_open()) {
		for (const COMPANY &company : servidor.companyList)
			file << company.name << _T(" ") << company.numFreeStocks << _T(" ") << company.pricePerStock << std::endl;
		
		file.close();

	} else {
		std::stringstream ss;
		ss << "Erro ao abrir o ficheiro '" << FILE_COMPANIES << "'";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << _T("Dados guardados no ficheiro '") << FILE_COMPANIES << _T("' com sucesso (2/") << TOTAL_FILES - 1 << _T(")") << std::endl;
}