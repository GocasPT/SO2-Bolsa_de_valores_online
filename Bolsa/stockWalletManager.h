#pragma once
#ifndef STOCK_WALLET_MANAGER_H
#define STOCK_WALLET_MANAGER_H

#include "SO2_Utils.h"

namespace SWManager {
	bool addStock(USER_DATA& user, COMPANY& company, DWORD numOfStocks);
	bool removeStock(USER_DATA& user, COMPANY& company, DWORD numOfStocks);
}

#endif // !STOCK_WALLET_MANAGER_Hs