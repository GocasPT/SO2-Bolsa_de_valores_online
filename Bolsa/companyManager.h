#pragma once
#ifndef COMPANY_MANAGER_H
#define COMPANY_MANAGER_H

#include "bolsa.h"

namespace CompanyManager {
	typedef enum {
		BUY,
		SELL
	} OPERATION;

	void config(BOLSA& servidor);
	void addCompany(BOLSA &servidor, std::TSTRING name, DWORD numOfStock, float pricePerStock);
	void listCompanies(BOLSA &servidor);
	COMPANY* getCompany(COMPANY_LIST& companyList, std::TSTRING name);
	void updateStock(BOLSA& servidor, std::TSTRING name, float pricePerStock);
	void updateStock(NOTIFY_DATA& notifyData, COMPANY& company, OPERATION opType);
	void pauseCompaniesOps(BOLSA& servidor, int time);
	DWORD WINAPI timerRoutine(LPVOID lpParam);
	void close(BOLSA& servidor);
}

#endif // !COMPANY_MANAGER_H