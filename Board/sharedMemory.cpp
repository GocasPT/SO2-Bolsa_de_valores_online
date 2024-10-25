﻿#include "sharedMemory.h"

void SharedMemory::connect(BOARD& board) {
    board.hSharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, SHARED_MEMORY_NAME);
    if (board.hSharedMemory == NULL) {
        std::_tcout << TEXT("[ERRO] Falha ao abrir a memória partilhada dos dados: ") << GetLastError() << std::endl;
        return;
    }

    board.sharedMemory = reinterpret_cast<SHARED_MEMORY*>(MapViewOfFile(board.hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SHARED_MEMORY)));
    if (board.sharedMemory == NULL) {
        std::_tcout << TEXT("[ERRO] Falha na associação da memória partilhada dos dados: ") << GetLastError() << std::endl;
        close(board);
        return;
    }

    board.hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
    if (board.hEvent == NULL) {
        std::_tcout << TEXT("[ERRO] Falha na abertura do evento para acesso à memória partilhada: ") << GetLastError() << std::endl;
        close(board);
        return;
    }

    board.sharedMemory->numBoards++;

    std::_tcout << TEXT("Configuração da memória partillhada concluída") << std::endl << std::endl;
}

bool SharedMemory::read(BOARD& board) {
    HANDLE events[2]{};
    events[0] = board.hEvent;
    events[1] = board.hExitEvent;

    if (WaitForMultipleObjects(2, events, FALSE, INFINITE) == WAIT_OBJECT_0) {
        CopyMemory(&board.data, board.sharedMemory, sizeof(SHARED_MEMORY));

        board.sharedMemory->boardsRead++;

        std::_tcout << TEXT("Boards Connected: ") << board.sharedMemory->numBoards << std::endl;
        std::_tcout << TEXT("Boards Read: ") << board.sharedMemory->boardsRead << std::endl;

        if (board.sharedMemory->boardsRead == board.sharedMemory->numBoards) {
            ResetEvent(board.hEvent);
        }
        return true;
    } else {
        std::_tcout << TEXT("A Bolsa fechou, prime ENTER para sair") << std::endl << std::endl;
        close(board);
        return false;
    }
}

void SharedMemory::close(BOARD& board) {
    std::_tcout << TEXT("A fechar a memória partilhada...") << std::endl;

    CloseHandle(board.hEvent);

    board.sharedMemory->numBoards--;
    UnmapViewOfFile(board.sharedMemory);

    CloseHandle(board.hSharedMemory);

    std::_tcout << TEXT("Memória partilhada fechada com sucesso") << std::endl << std::endl;
}