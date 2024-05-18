#include "SO2_Utils.h"
#include "sharedmemory.h"
#include "commands.h"
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "board.h"

void printCompany(COMPANY c) {
	std::_tcout << c.name << "\t" << c.numFreeStocks << "\t" << c.pricePerStock << std::endl;
}

void printTransaction(TRANSACTION t) {
	std::_tcout << t.username;
	if (t.buy) {
		std::_tcout << _T(" comprou ");
	}
	else {
		std::_tcout << _T(" vendeu ");
	}
	std::_tcout << t.numStocks << " stocks da empresa " << t.companyName << std::endl;
}

void printBoard(BOARD board) {
	system("cls");
	std::_tcout << "Name\nNumber of Free Stocks\nPrice Per Stock" << std::endl;
	for (DWORD i = 0; i < board.N; i++) {
		printCompany(board.data.companies[i]);
	}

	if (board.data.lastTransaction.numStocks != 0) {
		printTransaction(board.data.lastTransaction);
	}
}

int _tmain(int argc, std::TSTRING argv[]) {
	BOARD board{};
	board.isRunning = true;
	board.isPaused = false;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	if (argc == 1) {
		std::_tcout << _T("A usar 5 como o valor default de empresas a apresentar") << std::endl;
		board.N = 5;
	} else if ((board.N = stoi(argv[1])) == -1) {
		std::_tcout << TAG_ERROR << _T("Erro a dar parse ao argumento do número N") << std::endl;
		return 1;
	}

	board.hExitEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EXIT_EVENT_NAME);
	if (board.hExitEvent == NULL) {
		std::_tcout << std::endl << _T("Não existe nenhum programa Bolsa a ser executado") << std::endl;
		std::_tcout << _T("A terminar o programa...") << std::endl;
		return 1;
	}

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
		if (!SharedMemory::read(board)) {
			board.isRunning = false;
		}
		printBoard(board);
		std::_tcout << std::endl << std::endl;
	}

	WaitForSingleObject(board.hConsoleThread, INFINITE);
	CloseHandle(board.hConsoleThread);

	SharedMemory::close(board);
	return 0;
}