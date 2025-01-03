﻿#include "commands.h"
#include "namedPipe.h"
#include <sstream>

/**
 * \brief Rotina do cliente para a consola
 *
 * \param user - Estrutura com os dados do utilizador
 */
void cmd::consoleRoutine(CLIENTE& user) {
	std::TSTRING input;
	std::vector<std::TSTRING> args;
	std::_tstringstream ss;

	do {
		args.clear();
		ss.clear();

		std::_tcout << TAG_INPUT;
		std::getline(std::_tcin, input);

		if (input.empty())
			continue;

		ss << input;
		while (ss >> input)
			args.push_back(input);

		if (!validateCommand(user, args)) {
			std::_tcout << TAG_WARNING << _T("Comando inválido") << std::endl;
			continue;
		}

		WaitForSingleObject(user.hEventConsole, INFINITE);
	} while (input.compare(CMD_EXIT) != 0 && user.runnig);
}

/**
 * \brief Validar o comando introduzido pelo utilizador
 * 
 * \param userData - Estrutura com os dados do utilizador
 * \param args - Vetor com os argumentos do comando
 * 
 * \return true se o comando for válido, false caso contrário
 */
bool cmd::validateCommand(CLIENTE& userData, std::vector<std::TSTRING> args) {
	bool valid = false;

	if (!args[0].compare(CMD_HELP)) {
		std::_tcout << TAG_NORMAL << _T("Comandos disponíveis:") << std::endl <<
			_T("\t") << CMD_LOGIN << _T(" <username> <password>") << std::endl <<
			_T("\t") << CMD_LISTC << std::endl <<
			_T("\t") << CMD_BUY << _T(" <nome-empresa> <número-ações>") << std::endl <<
			_T("\t") << CMD_SELL << _T(" <nome-empresa> <número-ações>") << std::endl <<
			_T("\t") << CMD_BALANCE << std::endl <<
			_T("\t") << CMD_EXIT << std::endl << std::endl;

		SetEvent(userData.hEventConsole);

		valid = true;
	}

	/*
	* Comando: Login
	* Formato: login <username> <password>
	*/
	else if (!args[0].compare(CMD_LOGIN)) {
		if (args.size() != 3) {
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_LOGIN << _T(" <username> <password>") << std::endl << std::endl;
			SetEvent(userData.hEventConsole);
		}

		else if (userData.logged) {
			std::_tcout << TAG_WARNING << _T("Autenticação feita como ") << userData.name << _T(". Para trocar de utilizar, sair do programa com '") << CMD_EXIT << _T("'") << std::endl;
			SetEvent(userData.hEventConsole);
		}

		else
			NamedPipe::requestLogin(userData, args[1], args[2]);
			
		valid = true;
	}

	/*
	* Comando: Sair
	* Formato: exit
	*/
	else if (!args[0].compare(CMD_EXIT)) {
		if (args.size() != 1) {
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_EXIT << std::endl << std::endl;
		}

		SetEvent(userData.hEventConsole);
		userData.runnig = false;
		valid = true;
	}

	// Se não estiver autenticado, não pode executar os comandos seguintes
	else if (!userData.logged) {
		std::_tcout << TAG_WARNING << _T("Efetua o login primeiro") << std::endl;
		SetEvent(userData.hEventConsole);
		valid = true;
	}

	/*
	* Comando: Listar todas as empresas
	* Formato: listc
	*/
	else if (!args[0].compare(CMD_LISTC)) {
		if (args.size() != 1) {
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_LISTC << std::endl << std::endl;
			SetEvent(userData.hEventConsole);
		}

		else
			NamedPipe::requestList(userData);

		valid = true;
	}

	/*
	* Comando: Comprar ações
	* Formato: buy <nome-empresa> <número-ações>
	*/
	else if (!args[0].compare(CMD_BUY)) {
		if (args.size() != 3) {
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_BUY << _T(" <nome-empresa> <número-ações>") << std::endl << std::endl;
			SetEvent(userData.hEventConsole);
		}

		else
			NamedPipe::requestBuy(userData, args[1], std::stoi(args[2]));

		valid = true;
	}

	/*
	* Comando: Vender ações
	* Formato: sell <nome-empresa> <número-ações>
	*/
	else if (!args[0].compare(CMD_SELL)) {
		if (args.size() != 3) {
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_SELL << _T(" <nome-empresa> <número-ações>") << std::endl << std::endl;
			SetEvent(userData.hEventConsole);
		}

		else
			NamedPipe::requestSell(userData, args[1], std::stoi(args[2]));

		valid = true;
	}

	/*
	* Comando: Consultar saldo
	* Formato: balance
	*/
	else if (!args[0].compare(CMD_BALANCE)) {
		if (args.size() != 1) {
			std::_tcout << TAG_WARNING << _T("Formato errado") << std::endl << _T("\t") << CMD_BALANCE << std::endl << std::endl;
			SetEvent(userData.hEventConsole);
		}

		else
			NamedPipe::requestBalance(userData);

		valid = true;
	}

	return valid;
}