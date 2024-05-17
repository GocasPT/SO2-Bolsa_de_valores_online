#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H

#include "bolsa.h"

#define CMD_HELP _T("help")
#define CMD_DATA _T("showd")
#define CMD_ADDC _T("addc")
#define CMD_LISTC _T("listc")
#define CMD_STOCK _T("stock")
#define CMD_USERS _T("users")
#define CMD_PAUSE _T("pause")
#define CMD_CLOSE _T("close")

namespace cmd {
	void consoleRoutine(BOLSA& servidor);
	bool validateCommand(BOLSA& servidor, std::vector<std::TSTRING> args);
	void showData(BOLSA& servidor);
}

#endif // !COMMANDS_H
