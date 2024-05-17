#include "sharedMemory.h"

void SharedMemory::connect(BOARD& board) {
    board.hSharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, SHARED_MEMORY_NAME);
    if (board.hSharedMemory == NULL) {
        std::_tcout << _T("[ERRO] Falha ao abrir a memória partilhada dos dados: ") << GetLastError() << std::endl;
        return;
    }

    board.sharedMemory = reinterpret_cast<SHARED_MEMORY*>(MapViewOfFile(board.hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SHARED_MEMORY)));
    if (board.sharedMemory == NULL) {
        std::_tcout << _T("[ERRO] Falha na associação da memória partilhada dos dados: ") << GetLastError() << std::endl;
        close(board);
        return;
    }

    if (board.sharedMemory->numBoards == 10) {
        std::_tcout << _T("Já existem 10 boards abertas, tenta novamente mais tarde\nA terminar o programa...\n");
        close(board);
        exit(0);
    }

    board.hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
    if (board.hEvent == NULL) {
        std::_tcout << _T("[ERRO] Falha na abertura do evento para acesso à memória partilhada: ") << GetLastError() << std::endl;
        close(board);
        return;
    }

    board.sharedMemory->numBoards++;

    std::_tcout << _T("Configuração da memória partillhada concluída") << std::endl << std::endl;
}

void SharedMemory::read(BOARD& board) {
    WaitForSingleObject(board.hEvent, INFINITE);
	CopyMemory(&board.data, board.sharedMemory, sizeof(SHARED_MEMORY));
    //TODO: recebe a lista depois faz um 'crop' para o tamanho certo (recebe tudo, depois mostrar/guarda na memória a parte que quer)

    board.sharedMemory->boardsRead++;

    std::_tcout << _T("Boards Connected: ") << board.sharedMemory->numBoards << std::endl;
    std::_tcout << _T("Boards Read: ") << board.sharedMemory->boardsRead << std::endl;

    if (board.sharedMemory->boardsRead == board.sharedMemory->numBoards) {
        ResetEvent(board.hEvent);
    }
}

void SharedMemory::close(BOARD& board) {
    std::_tcout << _T("A fechar a memória partilhada...") << std::endl;

    CloseHandle(board.hEvent);

    board.sharedMemory->numBoards--;

    UnmapViewOfFile(board.sharedMemory);
    CloseHandle(board.hSharedMemory);

    std::_tcout << _T("Memória partilhada fechada com sucesso") << std::endl << std::endl;
}