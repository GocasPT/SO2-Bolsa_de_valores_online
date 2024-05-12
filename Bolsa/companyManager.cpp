#include "companyManager.h"
#include <sstream>

void CompanyManager::config(BOLSA& servidor) {
	servidor.timerData.isRunning = &servidor.isRunning;
	servidor.timerData.isPaused = &servidor.isPaused;
	servidor.timerData.time = 0;

	servidor.timerData.hEvent = CreateEvent(NULL, FALSE, FALSE, EVENT_TIMER);
	if (servidor.timerData.hEvent == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar o evento para o timer (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}


	servidor.hTimerThread = CreateThread(NULL, 0, CompanyManager::timerRoutine, &servidor.timerData, 0, NULL);
	if (servidor.hTimerThread == NULL) {
		std::stringstream ss;
		ss << "Erro ao criar a thread para o timer (" << GetLastError() << ")";
		throw std::runtime_error(ss.str());
	}

	std::_tcout << TAG_NORMAL << _T("Configuração do timer concluida") << std::endl;
}

void CompanyManager::addCompany(BOLSA& servidor, std::TSTRING name, DWORD numOfStock, DWORD pricePerStock) {
	if (getCompany(servidor.companyList, name) != nullptr) {
		std::_tcout << TAG_WARNING << _T("Empresa já existe") << std::endl;
		return;
	}

	COMPANY company;
	_tcscpy_s(company.name, name.c_str());
	company.numFreeStocks = numOfStock;
	company.pricePerStock = pricePerStock;

	servidor.companyList.push_back(company);

	std::_tcout << TAG_NORMAL << _T("Empresa adicionada com sucesso") << std::endl << _T("Nome: ") << name << _T(" | Nº de Ações: ") << company.numFreeStocks << _T(" | Preço por Ação: ") << company.pricePerStock << std::endl << std::endl;
}

void CompanyManager::listCompanies(BOLSA& servidor) {
	std::_tcout << TAG_NORMAL << _T("Listagem de Empresas:") << std::endl;
	
	if (servidor.companyList.empty()) {
		std::_tcout << _T("Nenhuma empresa") << std::endl;
		return;
	}

	for(auto it = servidor.companyList.begin(); it != servidor.companyList.end(); it++)
		std::_tcout << _T("Nome: ") << it->name << _T(" | Nº de Ações: ") << it->numFreeStocks << _T(" | Preço por Ação: ") << it->pricePerStock << std::endl;

	std::_tcout << std::endl;
}

COMPANY* CompanyManager::getCompany(COMPANY_LIST& companyList, std::TSTRING name) {
	auto it = std::find_if(companyList.begin(), companyList.end(),
		[name](COMPANY company) { return name.compare(company.name) == 0; });

	if (it == companyList.end())
		return nullptr;

	return &(*it);
}

void CompanyManager::updateStock(COMPANY& company, CompanyManager::OPERATION opType) {
	float variation = (rand() % 100) / 100.0f;

	switch (opType) {
		case CompanyManager::BUY:
			company.pricePerStock += variation;
			break;

		case CompanyManager::SELL:
			company.pricePerStock -= variation;
			break;
	}

	std::_tcout << TAG_NORMAL << _T("Preço das ações atualizado com sucesso") << std::endl << _T("Nome: ") << company.name << _T(" | Preço por Ação: ") << company.pricePerStock << std::endl << std::endl;

	//TODO: notify users (set event to send message)
}

void CompanyManager::pauseCompaniesOps(BOLSA& servidor, int time) {
	servidor.timerData.time = time;
	SetEvent(servidor.timerData.hEvent);
}

DWORD WINAPI CompanyManager::timerRoutine(LPVOID lpParam) {
	TIMER_DATA* data = (TIMER_DATA*)lpParam;

	std::_tcout << std::endl << _T("Thread do timer iniciada") << std::endl;

	while (*(data->isRunning)) {
		WaitForSingleObject(data->hEvent, INFINITE);

		if (!*(data->isRunning))
			break;

		*(data->isPaused) = true;
		std::_tcout << std::endl << _T("Pausa das operações de compra e venda de ações por ") << data->time << _T(" segundos...") << std::endl;
		//TODO: unlock console here

		Sleep(1000 * data->time);

		*(data->isPaused) = false;
		std::_tcout << std::endl << _T("Operações de compra e venda de ações retornadas") << std::endl;
	}

	std::_tcout << _T("A encerrar a thread do timer...") << std::endl;

	return 0;
}


void CompanyManager::close(BOLSA& servidor) {
	std::_tcout << _T("A fechar a thread e recursos do timer...") << std::endl;

	SetEvent(servidor.timerData.hEvent);
	WaitForSingleObject(servidor.hTimerThread, INFINITE);
	CloseHandle(servidor.hTimerThread);
	CloseHandle(servidor.timerData.hEvent);

	std::_tcout << TAG_NORMAL << _T("Thread e recursos do timer fechado com sucesso") << std::endl << std::endl;
}