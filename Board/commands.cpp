#include "commands.h"
#include <sstream>

DWORD WINAPI cmd::consoleRoutine(LPVOID param) {
    BOARD& board = *static_cast<BOARD*>(param);

    std::TSTRING input;
    std::vector<std::TSTRING> args;
    std::_tstringstream ss;

    do {
        args.clear();
        ss.clear();

        std::_tcout << _T(">> ");
        std::getline(std::_tcin, input);

        ss << input;
        while (ss >> input)
            args.push_back(input);

        if (input.empty())
            continue;

    } while (input.compare(CMD_EXIT) && board.isRunning);

    board.isRunning = false;
    SharedMemory::close(board);
    CloseHandle(board.hConsoleThread);
    exit(0);

    return 0;
}
