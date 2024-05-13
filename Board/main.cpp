#include "SO2_Utils.h"
#include "sharedmemory.h"
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
	while (board.isRunning){
		SharedMemory::read(board);
		printBoard(board);
		std::_tcout << std::endl << std::endl;
		Sleep(1000);
	}

	return 0;
}