#pragma once
#ifndef COMPANY_MANAGER_H
#define COMPANY_MANAGER_H

#include "bolsa.h"

namespace CompanyManager {
	void addCompany(BOLSA &servidor, std::TSTRING name, std::TSTRING numOfStock, std::TSTRING pricePerStock);
	void listCompanies(BOLSA &servidor);
	COMPANY* getCompany(BOLSA &servidor, std::TSTRING name);
	void updateStock(BOLSA &servidor, std::TSTRING name, std::TSTRING pricePerStock);
}

#endif // !COMPANY_MANAGER_H