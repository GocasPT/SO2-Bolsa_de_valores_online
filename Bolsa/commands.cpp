#include "commands.h"

void cmd::consoleRoutine(BOLSA& servidor) {
	std::TSTRING input;
	std::vector<std::TSTRING> args;

	//TODO: change this for flag (any error, break loop → for safe) 
	do {
		std::tcout << _T(">> ");
		std::getline(std::tcin, input);

		//TODO: slipt input into args → args[0] = cmd, args[1] = arg1, args[2] = arg2, etc

		//TODO
		/*if (!validateCommand(args)) {
			std::tcout << _T("Comando inválido") << std::endl;
			continue;
		}*/

		//TODO: PLACEHOLDER
		std::tcout << _T("Comando: \'") << input << _T("\'") << std::endl;

	} while (input.compare(CMD_CLOSE));
}

BOOL cmd::validateCommand(std::vector<std::TSTRING> args) {
	/*
	* Comando: Acrescentar uma empresa
	* Formato: addc <nome> <capital> <n_acoes>
	*/
	if (args[0].compare(CMD_ADDC)) {
		if (args.size() != 4) {
			std::tcout << _T("Formato errado") << std::endl;
			return FALSE;
		}
		else {
			//TODO: execute command
			return TRUE;
		}
	}

	/*
	* Comando: Listar todas as empresas
	* Formato: listc
	*/
	else if (args[0].compare(CMD_LISTC)) {
		if (args.size() != 1) {
			std::tcout << _T("Formato errado") << std::endl;
			return FALSE;
		}
		else {
			//TODO: execute command
			return TRUE;
		}
	}

	/*
	* Comando: Redefinir custo das ações de uma empresa
	* Formato: stock <nome-empresa> <preço-ação>
	*/
	else if (args[0].compare(CMD_STOCK)) {
		if (args.size() != 3) {
			std::tcout << _T("Formato errado") << std::endl;
			return FALSE;
		}
		else {
			//TODO: execute command
			return TRUE;
		}
	}

	/*
	* Comando: Listar utilizadores
	* Formato: users
	*/
	else if (args[0].compare(CMD_USERS)) {
		if (args.size() != 1) {
			std::tcout << _T("Formato errado") << std::endl;
			return FALSE;
		}
		else {
			//TODO: execute command
			return TRUE;
		}
	}

	/*
	* Comando: Pausar as operações de compra e venda
	* Formato: pause <número-segundos>
	*/
	else if (args[0].compare(CMD_PAUSE)) {
		if (args.size() != 2) {
			std::tcout << _T("Formato errado") << std::endl;
			return FALSE;
		}
		else {
			//TODO: execute command
			return TRUE;
		}
	}

	/*
	* Comando: Encerrar a plataforma
	* Formato: close
	*/
	else if (args[0].compare(CMD_CLOSE)) {
		if (args.size() != 1) {
			std::tcout << _T("Formato errado") << std::endl;
			return FALSE;
		}
		else
			return TRUE;
	}

	return FALSE;
}