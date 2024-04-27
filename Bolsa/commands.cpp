#include "commands.h"
#include "companyManager.h"
#include "userManager.h"
#include <sstream>

void cmd::consoleRoutine(BOLSA& servidor) {
	std::TSTRING input;
	std::vector<std::TSTRING> args;
	std::wstringstream ss; //TODO: change to macro (tstringstream → wstringstream or stringstream)

	//TODO: change this for flag (any error, break loop → for safe) 
	do {
		args.clear();
		ss.clear();

		std::_tcout << _T(">> ");
		std::getline(std::_tcin, input);

		ss << input;
		while (ss >> input)
			args.push_back(input);

		if (!validateCommand(servidor, args)) {
			std::_tcout << TAG_WARNING << _T("Comando inválido") << std::endl;
			continue;
		}
	} while (input.compare(CMD_CLOSE));
}

BOOL cmd::validateCommand(BOLSA& servidor, std::vector<std::TSTRING> args) {
	/*
	* Comando: Acrescentar uma empresa
	* Formato: addc <nome-empresa> <número-ações> <preço-ação>
	*/
	if (!args[0].compare(CMD_ADDC)) {
		if (args.size() != 4)
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") <<
			CMD_ADDC << _T(" <nome-empresa> <número-ações> <preço-ação>") << std::endl << std::endl;

		else
			CompanyManager::addCompany(servidor, args[1], args[2], args[3]);

		return TRUE;
	}

	/*
	* Comando: Listar todas as empresas
	* Formato: listc
	*/
	else if (!args[0].compare(CMD_LISTC)) {
		if (args.size() != 1)
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") <<
			CMD_LISTC << std::endl << std::endl;

		else
			CompanyManager::listCompanies(servidor);

		return TRUE;
	}

	/*
	* Comando: Redefinir custo das ações de uma empresa
	* Formato: stock <nome-empresa> <preço-ação>
	*/
	else if (!args[0].compare(CMD_STOCK)) {
		if (args.size() != 3)
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") <<
				CMD_STOCK << _T(" <nome-empresa> <preço-ação>") << std::endl << std::endl;

		else
			CompanyManager::updateStock(servidor, args[1], args[2]);

		return TRUE;
	}

	/*
	* Comando: Listar utilizadores
	* Formato: users
	*/
	else if (!args[0].compare(CMD_USERS)) {
		if (args.size() != 1)
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") <<
				CMD_USERS << std::endl << std::endl;

		else
			UserManager::listUsers(servidor);

		
		return TRUE;
	}

	/*
	* Comando: Pausar as operações de compra e venda
	* Formato: pause <número-segundos>
	*/
	else if (!args[0].compare(CMD_PAUSE)) {
		if (args.size() != 2)
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") <<
				CMD_PAUSE << _T(" <número-segundos>") << std::endl << std::endl;

		else {
			//TODO: execute command
			//TODO: enable flag to pause operations, wait for seconds and disable flag (note: dont enter the cs)
		}
		
		return TRUE;
	}

	/*
	* Comando: Encerrar a plataforma
	* Formato: close
	*/
	else if (!args[0].compare(CMD_CLOSE)) {
		if (args.size() != 1)
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") <<
				CMD_CLOSE << std::endl << std::endl;

		return TRUE;
	}

	return FALSE;
}