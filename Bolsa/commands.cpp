#include "commands.h"
#include "companyManager.h"
#include "userManager.h"
#include <sstream>

void cmd::consoleRoutine(BOLSA& servidor) {
	std::TSTRING input;
	std::vector<std::TSTRING> args;
	std::_tstringstream ss;

	do {
		args.clear();
		ss.clear();

		std::_tcout << TAG_INPUT;
		std::getline(std::_tcin, input);

		ss << input;
		while (ss >> input)
			args.push_back(input);

		if (input.empty())
			continue;

		if (!validateCommand(servidor, args)) {
			std::_tcout << TAG_WARNING << _T("Comando inválido") << std::endl;
			continue;
		}

	} while (input.compare(CMD_CLOSE) && servidor.isRunning);
}

bool cmd::validateCommand(BOLSA& servidor, std::vector<std::TSTRING> args) {
	try {
		/*
		* Comando: Ajuda
		* Formato: help
		*/
		if (!args[0].compare(CMD_HELP)) {
			std::_tcout << TAG_NORMAL << _T("Comandos disponíveis:") << std::endl <<
				_T("\t") << CMD_DATA << std::endl <<
				_T("\t") << CMD_ADDC << _T(" <nome-empresa> <número-ações> <preço-ação>") << std::endl <<
				_T("\t") << CMD_LISTC << std::endl <<
				_T("\t") << CMD_STOCK << _T(" <nome-empresa> <preço-ação>") << std::endl <<
				_T("\t") << CMD_USERS << std::endl <<
				_T("\t") << CMD_PAUSE << _T(" <número-segundos>") << std::endl <<
				_T("\t") << CMD_CLOSE << std::endl << std::endl;

			return true;
		}

		else if (!args[0].compare(CMD_DATA)) {
			if (args.size() != 1)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_DATA << std::endl << std::endl;

			else
				showData(servidor);

			return true;

		}

		/*
		* Comando: Acrescentar uma empresa
		* Formato: addc <nome-empresa> <número-ações> <preço-ação>
		*/
		else if (!args[0].compare(CMD_ADDC)) {
			if (args.size() != 4)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_ADDC << _T(" <nome-empresa> <número-ações> <preço-ação>") << std::endl << std::endl;

			else
				CompanyManager::addCompany(servidor, args[1], std::stoi(args[2]), std::stof(args[3]));

			return true;
		}

		/*
		* Comando: Listar todas as empresas
		* Formato: listc
		*/
		else if (!args[0].compare(CMD_LISTC)) {
			if (args.size() != 1)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_LISTC << std::endl << std::endl;

			else
				CompanyManager::listCompanies(servidor);

			return true;
		}

		/*
		* Comando: Redefinir custo das ações de uma empresa
		* Formato: stock <nome-empresa> <preço-ação>
		*/
		else if (!args[0].compare(CMD_STOCK)) {
			if (args.size() != 3)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_STOCK << _T(" <nome-empresa> <preço-ação>") << std::endl << std::endl;

			else
				CompanyManager::updateStock(servidor, args[1], std::stof(args[2]));

			return true;
		}

		/*
		* Comando: Listar utilizadores
		* Formato: users
		*/
		else if (!args[0].compare(CMD_USERS)) {
			if (args.size() != 1)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_USERS << std::endl << std::endl;

			else
				UserManager::listUsers(servidor);


			return true;
		}

		/*
		* Comando: Pausar as operações de compra e venda
		* Formato: pause <número-segundos>
		*/
		else if (!args[0].compare(CMD_PAUSE)) {
			if (args.size() != 2)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_PAUSE << _T(" <número-segundos>") << std::endl << std::endl;

			else
				CompanyManager::pauseCompaniesOps(servidor, std::stoi(args[1]));

			return true;
		}

		/*
		* Comando: Encerrar a plataforma
		* Formato: close
		*/
		else if (!args[0].compare(CMD_CLOSE)) {
			if (args.size() != 1)
				std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_CLOSE << std::endl << std::endl;

			return true;
		}
	} catch (std::invalid_argument e) {
		std::_tcout << TAG_ERROR << _T("Argumentos inválidos. Verifica o tipo do argumento pedido") << std::endl;
		return true;
	}
	
	return false;
}

void cmd::showData(BOLSA& servidor) {
	std::_tcout << _T("Threads de clientes (") << servidor.tDataList.size() << _T(")") << std::endl;

	if (servidor.tDataList.empty()) {
		std::_tcout << _T("Não existem threads de clientes a correr") << std::endl;
		return;
	} else {
		for (auto &data : servidor.tDataList) {
			std::_tcout << _T("Thread ") << data.tID << _T(" a atender o cliente ") << data.myUser->name << std::endl;
		}
	}

	std::_tcout << std::endl << _T("Thread timer ") << ( servidor.isPaused ? _T("ativada") : _T("em espera") ) << std::endl;
}
