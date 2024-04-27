#include "userManager.h"
#include <algorithm>

//TODO: check
BOOL UserManager::validateUser(BOLSA& servidor, USER user) {
	auto it = std::find_if(servidor.userList.begin(), servidor.userList.end(),
		[user](USER u) { return u.name == user.name && u.password == user.password; });
	if (it == servidor.userList.end())
		return false;
	else
		return true;
}

//TODO: check
void UserManager::addUser(BOLSA& bolsa, USER newUser) {
	std::vector<USER> &userList = bolsa.userList;
	std::queue<USER> &userQueue = bolsa.userQueue;

	if (userList.size() < bolsa.maxUsers)
		userList.push_back(newUser);
	else
		userQueue.push(newUser);
}

//TODO: check
void UserManager::removeUser(BOLSA& bolsa, std::TSTRING userName) {
	std::vector<USER>& userList = bolsa.userList;
	std::queue<USER>& userQueue = bolsa.userQueue;

	auto it = std::find_if(userList.begin(), userList.end(),
		[userName](USER user) { return user.name == userName; });
	if (it != userList.end())
		userList.erase(it);

	if (!userQueue.empty()) {
		userList.push_back(userQueue.front());
		userQueue.pop();
	}
}

void UserManager::listUsers(BOLSA& bolsa) {
	std::tcout << _T("Users:") << std::endl;

	if (bolsa.userList.empty()) {
		std::tcout << _T("Nenhum cliente") << std::endl;
		return;
	}

	for (auto it = bolsa.userList.begin(); it != bolsa.userList.end(); it++)
		std::tcout << _T("Nome: ") << it->name <<
			_T(" | Saldo: ") << it->balance <<
			_T(" [") << (it->connected ? _T("Online") : _T("Offline")) << _T("]")
			<< std::endl;

	std::tcout << std::endl;
}

//TODO: check
USER* UserManager::getUser(BOLSA& bolsa, std::TSTRING userName) {
	std::vector<USER>& userList = bolsa.userList;

	auto it = std::find_if(userList.begin(), userList.end(),
		[userName](USER user) { return user.name == userName; });
	if (it != userList.end())
		return &(*it);
	else
		return nullptr;
}

//TODO: check
void UserManager::releaseUsers(BOLSA& bolsa) {
	std::vector<USER>& userList = bolsa.userList;
	std::queue<USER>& userQueue = bolsa.userQueue;

	for (auto it = userList.begin(); it != userList.end(); it++)
		//TODO: send message to user to disconnect
		;

	while (!userQueue.empty()) {
		//TODO: send message to user to disconnect
	}
}