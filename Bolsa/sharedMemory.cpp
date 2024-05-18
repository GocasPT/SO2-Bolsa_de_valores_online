#include "sharedMemory.h"

void SharedMemory::config(BOLSA& servidor) {
	std::_tcout << _T("A configurar a memória partilhada para mandar as board(GUI)s...") << std::endl;

	servidor.notifyData.company = nullptr;

	servidor.hSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SHARED_MEMORY), SHARED_MEMORY_NAME);
	if (servidor.hSharedMemory == NULL) {
		std::_tcout << _T("[ERRO] Falha na criação da memória partilhada dos dados: ") << GetLastError() << std::endl;
		return;
	}

	servidor.sharedMemory = reinterpret_cast<SHARED_MEMORY*>(MapViewOfFile(servidor.hSharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SHARED_MEMORY)));
	if (servidor.sharedMemory == NULL) {
		std::_tcout << _T("[ERRO] Falha na associação da memória partilhada dos dados: ") << GetLastError() << std::endl;
		close(servidor);
		return;
	}

	servidor.hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
	if (servidor.hEvent == NULL) {
		std::_tcout << _T("[ERRO] Falha na criação do evento: ") << GetLastError() << std::endl;
		close(servidor);
		return;
	} 

	servidor.sharedMemory->numBoards = 0;
	servidor.sharedMemory->boardsRead = 0;

	std::_tcout << TAG_NORMAL << _T("Configuração da memória partillhada concluída, disponível para a conexão das board(GUI)s") << std::endl << std::endl;
}

bool compareStockPrice(const COMPANY& c1, const COMPANY& c2) {
	return c1.pricePerStock > c2.pricePerStock;
}

void SharedMemory::update(BOLSA &servidor) {
	ZeroMemory(servidor.sharedMemory->companies, sizeof(COMPANY) * MAX_COMPANIES);
	std::copy(servidor.companyList.begin(), servidor.companyList.end(), servidor.sharedMemory->companies);

	std::sort(servidor.sharedMemory->companies, servidor.sharedMemory->companies + MAX_COMPANIES, compareStockPrice);

	if (servidor.notifyData.company) {
		_tcscpy_s(servidor.sharedMemory->lastTransaction.companyName, servidor.notifyData.company->name);
		_tcscpy_s(servidor.sharedMemory->lastTransaction.username, servidor.notifyData.user.name);
		servidor.sharedMemory->lastTransaction.numStocks = servidor.notifyData.numStocks;
		servidor.sharedMemory->lastTransaction.buy = servidor.notifyData.oldPrice < servidor.notifyData.company->pricePerStock;
	}

	servidor.sharedMemory->boardsRead = 0;
	SetEvent(servidor.hEvent);
}

void SharedMemory::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar a memória partilhada...") << std::endl;

	CloseHandle(servidor.hEvent);

	UnmapViewOfFile(servidor.sharedMemory);
	CloseHandle(servidor.hSharedMemory);

	std::_tcout << TAG_NORMAL << _T("Memória partilhada fechada com sucesso") << std::endl << std::endl;
}