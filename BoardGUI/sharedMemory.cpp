#include "sharedMemory.h"

void SharedMemory::connect(BOARD& board) {
    std::wstringstream ss;

    board.hSharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, SHARED_MEMORY_NAME);
    if (board.hSharedMemory == NULL) {
		ss << "[ERRO] Falha ao abrir a memória partilhada dos dados: " << GetLastError() << std::endl;
		throw ss.str();
    }

    board.sharedMemory = reinterpret_cast<SHARED_MEMORY*>(MapViewOfFile(board.hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SHARED_MEMORY)));
    if (board.sharedMemory == NULL) {
		ss << "[ERRO] Falha na associação da memória partilhada dos dados: " << GetLastError() << std::endl;
		throw ss.str();
    }

    board.hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
    if (board.hEvent == NULL) {
		ss << "[ERRO] Falha na abertura do evento para acesso à memória partilhada: " << GetLastError() << std::endl;
		throw ss.str();
    }

    board.sharedMemory->numBoards++;
}

bool SharedMemory::read(BOARD& board) {
    HANDLE events[2]{};
    events[0] = board.hEvent;
    events[1] = board.hExitEvent;

    if (WaitForMultipleObjects(2, events, FALSE, INFINITE) == WAIT_OBJECT_0) {
        CopyMemory(&board.data, board.sharedMemory, sizeof(SHARED_MEMORY));
        //TODO: recebe a lista depois faz um 'crop' para o tamanho certo (recebe tudo, depois mostrar/guarda na memória a parte que quer)

        board.sharedMemory->boardsRead++;

        if (board.sharedMemory->boardsRead == board.sharedMemory->numBoards) {
            ResetEvent(board.hEvent);
        }
        return true;
    } else {
        close(board);
        return false;
    }
}

void SharedMemory::close(BOARD& board) {
    CloseHandle(board.hEvent);

    if (!board.sharedMemory) {
        board.sharedMemory->numBoards--;
        UnmapViewOfFile(board.sharedMemory);
    }

    CloseHandle(board.hSharedMemory);
}