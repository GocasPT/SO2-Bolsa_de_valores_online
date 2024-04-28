#include "userManager.h"
#include <algorithm>

//TODO: check
bool UserManager::validateUser(const std::vector<USER> &userList, USER user) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[user](USER userItem) { return _tcscmp(userItem.name, user.name) == 0 && _tcscmp(userItem.password, user.password) == 0; });
	if (it == userList.end())
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
	std::_tcout << _T("Users:") << std::endl;

	if (bolsa.userList.empty()) {
		std::_tcout << _T("Nenhum cliente") << std::endl;
		return;
	}

	for (auto it = bolsa.userList.begin(); it != bolsa.userList.end(); it++)
		std::_tcout << _T("Nome: ") << it->name <<
			_T(" | Saldo: ") << it->balance <<
			_T(" [") << (it->connected ? _T("Online") : _T("Offline")) << _T("]")
			<< std::endl;

	std::_tcout << std::endl;
}

//TODO: check
USER& UserManager::getUser(std::vector<USER> &userList, std::TSTRING userName) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[userName](USER user) { return userName.compare(user.name) == 0; });
	
	if (it == userList.end())
		throw std::runtime_error("User not found"); //TODO: improve exception
	else
		return *it;
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