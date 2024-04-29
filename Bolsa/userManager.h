#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "bolsa.h"

namespace UserManager {
	bool validateUser(const USER_LIST& userList, USER user);
	bool addUser(BOLSA& servidor, USER* user);
	USER* removeUser(USER_LIST& userList, USER_QUEUE& userQueue, USER* user);
	void listUsers(const BOLSA& servidor);
	USER& getUser(USER_LIST& userList, std::TSTRING userName);
	void releaseUsers(BOLSA& servidor);
}

#endif // !USER_MANAGER