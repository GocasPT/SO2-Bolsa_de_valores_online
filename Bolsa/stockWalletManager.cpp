#include "stockWalletManager.h"
#include "companyManager.h"

bool SWManager::userHaveStock(USER_DATA& user, COMPANY& company) {
	for (DWORD i = 0; i < user.walletSize; i++)
		if (!_tcscmp(user.wallet[i].companyName, company.name))
			return true;
	return false;
}

bool SWManager::addStock(USER_DATA& user, COMPANY& company, DWORD numOfStocks) {
	if (numOfStocks * company.pricePerStock > user.balance) {
		return false;
	}

	bool updatedStock = false;
	for (DWORD i = 0; i < user.walletSize; i++)
		if (!_tcscmp(user.wallet[i].companyName, company.name)) {
			user.wallet[i].numStocks += numOfStocks;
			updatedStock = true;
			break;
		}

	if (!updatedStock) {
		if (user.walletSize == MAX_STOCKS) return false;

		STOCK_ITEM stock;
		_tcscpy_s(stock.companyName, company.name);
		stock.numStocks = numOfStocks;
		stock.pricePerStock = company.pricePerStock;
		user.wallet[user.walletSize++] = stock;
	}
	
	company.numFreeStocks -= numOfStocks;
	user.balance -= numOfStocks * company.pricePerStock;

	//CompanyManager::updateStock(company, CompanyManager::BUY);

	return true;
}

bool SWManager::removeStock(USER_DATA& user, COMPANY& company, DWORD numOfStocks) {
	for (DWORD i = 0; i < user.walletSize; i++)
		if (!_tcscmp(user.wallet[i].companyName, company.name)) {
			if (user.wallet[i].numStocks >= numOfStocks) {
				if (user.wallet[i].numStocks < numOfStocks)
					return false;

				user.wallet[i].numStocks -= numOfStocks;
				company.numFreeStocks += numOfStocks;
				user.balance += numOfStocks * company.pricePerStock;
				
				if (user.wallet[i].numStocks == 0) {
					for (DWORD j = i; j < user.walletSize - 1; j++)
						user.wallet[j] = user.wallet[j + 1];
					user.walletSize--;
				}

				return true;
			}
		}

	return false;
}