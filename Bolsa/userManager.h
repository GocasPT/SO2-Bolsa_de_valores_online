#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "bolsa.h"

namespace UserManager {
	BOOL validateUser(USER user);
	void addUser(BOLSA& bolsa, USER newUser);
	void removeUser(BOLSA& bolsa, std::TSTRING userName);
	USER* getUser(BOLSA& bolsa, std::TSTRING userName);
	void releaseUsers(BOLSA& bolsa);
}

#endif // !USER_MANAGER