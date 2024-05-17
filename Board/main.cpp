#include "SO2_Utils.h"
#include "sharedmemory.h"
#include "commands.h"
#include <fcntl.h>
#include <io.h>
#include <iostream>

void printCompany(COMPANY c) {
	std::_tcout << c.name << "\t" << c.numFreeStocks << "\t" << c.pricePerStock << std::endl;
}

void printBoard(BOARD board) {
	std::_tcout << "Name\nNumber of Free Stocks\nPrice Per Stock" << std::endl;
	for (DWORD i = 0; i < board.data.numCompanies; i++) {
		printCompany(board.data.companies[i]);
	}
}

int _tmain(int argc, std::TSTRING argv[]) {
	// VARIABLES
	BOARD board{};
	board.isRunning = true;
	board.isPaused = false;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	SharedMemory::connect(board);

	SharedMemory::read(board);
	printBoard(board);
	std::_tcout << std::endl << std::endl;

	std::_tcout << _T("Escreve exit para sair") << std::endl;

	board.hConsoleThread = CreateThread(NULL, 0, cmd::consoleRoutine, &board, 0, NULL);
	if (board.hConsoleThread == NULL) {
		std::cerr << TAG_ERROR << "Erro a criar a thread consoleRoutine" << std::endl;
		return 1;
	}

	while (board.isRunning){
		SharedMemory::read(board);
		printBoard(board);
		std::_tcout << std::endl << std::endl;
	}

	WaitForSingleObject(board.hConsoleThread, INFINITE);
	CloseHandle(board.hConsoleThread);

	SharedMemory::close(board);
	return 0;
}