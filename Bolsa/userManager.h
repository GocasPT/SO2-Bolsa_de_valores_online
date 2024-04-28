#pragma once
#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "bolsa.h"

namespace UserManager {
	bool validateUser(const std::vector<USER> &userList, USER user);
	void addUser(BOLSA& servidor, USER newUser);
	void removeUser(BOLSA& servidor, std::TSTRING userName);
	void listUsers(BOLSA& servidor);
	USER& getUser(std::vector<USER> &userList, std::TSTRING userName);
	void releaseUsers(BOLSA& servidor);
}

#endif // !USER_MANAGER