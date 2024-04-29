#include "userManager.h"
#include <algorithm>

//TODO: check

bool UserManager::validateUser(const USER_LIST& userList, USER user) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[user](USER userItem) { return _tcscmp(userItem.name, user.name) == 0 && _tcscmp(userItem.password, user.password) == 0; });
	if (it == userList.end())
		return false;
	else
		return true;
}

//TODO: check
bool UserManager::addUser(BOLSA& servidor, USER* user) {
	if (servidor.hUsersList.size() < servidor.maxUsers) {
		servidor.hUsersList.push_back(user->hReciverPipe);
		user->connected = true;
		return true;
	} else {
		servidor.hUsersQueue.push(user);
		user->inQueue = true;
		return false;
	}
}

//TODO: check
USER* UserManager::removeUser(USER_LIST& userList, USER_QUEUE& userQueue, USER* user) {
	auto it = std::find_if(userList.begin(), userList.end(),
				[user](USER userItem) { return _tcscmp(userItem.name, user->name) == 0; });

	if (it == userList.end()) {
		throw std::runtime_error("User not found"); //TODO: improve exception
	}

	it->connected = false;
	CloseHandle(it->hReciverPipe);

	if (userQueue.empty())
		return nullptr;

	user = userQueue.front();
	user->connected = true;
	user->inQueue = false;
	userQueue.pop();

	return user;
}

void UserManager::listUsers(const BOLSA& servidor) {
	std::_tcout << _T("Users:") << std::endl;

	if (servidor.userList.empty()) {
		std::_tcout << _T("Nenhum cliente") << std::endl;
		return;
	}

	for (auto it = servidor.userList.begin(); it != servidor.userList.end(); it++)
		std::_tcout << _T("Nome: ") << it->name <<
			_T(" | Saldo: ") << it->balance <<
			_T(" [") << (it->connected ? _T("Online") : it->inQueue ? _T("Fila de espera") : _T("Offline")) << _T("]")
			<< std::endl;

	std::_tcout << std::endl;
}

//TODO: check
USER& UserManager::getUser(USER_LIST& userList, std::TSTRING userName) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[userName](USER user) { return userName.compare(user.name) == 0; });
	
	if (it == userList.end())
		throw std::runtime_error("User not found"); //TODO: improve exception
	else
		return *it;
}

//TODO: check
//TODO: we need this?
void UserManager::releaseUsers(BOLSA& servidor) {
	USER_LIST& userList = servidor.userList;
	USER_QUEUE& hUsersQueue = servidor.hUsersQueue;

	for (auto it = userList.begin(); it != userList.end(); it++)
		//TODO: send message to user to disconnect
		;

	while (!hUsersQueue.empty()) {
		//TODO: send message to user to disconnect
	}
}