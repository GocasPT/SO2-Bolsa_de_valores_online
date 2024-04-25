#include "userManager.h"
#include <algorithm>

//TODO: check
BOOL validateUser(USER user) {
	return true;
}

//TODO: check
void addUser(BOLSA& bolsa, USER newUser) {
	std::vector<USER> &userList = bolsa.userList;
	std::queue<USER> &userQueue = bolsa.userQueue;

	if (userList.size() < bolsa.maxUsers)
		userList.push_back(newUser);
	else
		userQueue.push(newUser);
}

//TODO: check
void removeUser(BOLSA& bolsa, std::TSTRING userName) {
	std::vector<USER>& userList = bolsa.userList;
	std::queue<USER>& userQueue = bolsa.userQueue;

	auto it = std::find_if(userList.begin(), userList.end(), [userName](USER user) { return user.name == userName; });
	if (it != userList.end())
		userList.erase(it);

	if (!userQueue.empty()) {
		userList.push_back(userQueue.front());
		userQueue.pop();
	}
}

//TODO: check
USER* getUser(BOLSA& bolsa, std::TSTRING userName) {
	std::vector<USER>& userList = bolsa.userList;

	auto it = std::find_if(userList.begin(), userList.end(), [userName](USER user) { return user.name == userName; });
	if (it != userList.end())
		return &(*it);
	else
		return nullptr;
}

//TODO: check
void releaseUsers(BOLSA& bolsa) {
	std::vector<USER>& userList = bolsa.userList;
	std::queue<USER>& userQueue = bolsa.userQueue;

	for (auto it = userList.begin(); it != userList.end(); it++)
		//TODO: send message to user to disconnect
		;

	while (!userQueue.empty()) {
		//TODO: send message to user to disconnect
	}
}