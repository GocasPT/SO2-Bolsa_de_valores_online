#ifndef COMMANDS_BOARD_H
#define COMMANDS_BOARD_H

#include "board.h"
#include "sharedMemory.h"
#include <Windows.h>

namespace cmd {
    static const std::TSTRING CMD_EXIT = _T("exit");

    DWORD WINAPI consoleRoutine(LPVOID param);
}

#endif // COMMANDS_BOARD_H
