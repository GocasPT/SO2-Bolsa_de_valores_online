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

    board.hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, EVENT_NAME);
    if (board.hEvent == NULL) {
        std::_tcout << _T("[ERRO] Falha na abertura do evento para acesso à memória partilhada: ") << GetLastError() << std::endl;
        close(board);
        return;
    }
    std::_tcout << _T("Configuração da memória partillhada concluída") << std::endl << std::endl;
}

void SharedMemory::read(BOARD& board) {
    WaitForSingleObject(board.hEvent, INFINITE);
    board.data = *(board.sharedMemory);
}

void SharedMemory::close(BOARD& board) {
    std::_tcout << _T("A fechar a memória partilhada...") << std::endl;

    CloseHandle(board.hEvent);

    UnmapViewOfFile(board.sharedMemory);
    CloseHandle(board.hSharedMemory);

    std::_tcout << _T("Memória partilhada fechada com sucesso") << std::endl << std::endl;
}