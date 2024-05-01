#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H

#include "cliente.h"
#include <vector>

#define CMD_HRLP _T("help")
#define CMD_LOGIN _T("login")
#define CMD_LISTC _T("listc")
#define CMD_BUY _T("buy")
#define CMD_SELL _T("sell")
#define CMD_BALANCE _T("balance")
#define CMD_EXIT _T("exit")

namespace cmd {
	bool validateCommand(CLIENTE &userData, std::vector<std::TSTRING> args);
	void consoleRoutine(CLIENTE &user);
}

#endif // !COMMANDS_H
