#include "userManager.h"
#include <algorithm>

//TODO: check
/**
 * @brief Validar as cred�nciais do utilizador.
 *
 * @param userList - Lista de utilizadores.
 * @param user - Utilizador a validar.
 * 
 * @return true se o utilizador for v�lido, false caso contr�rio.
 */
bool UserManager::validateUser(const USER_LIST& userList, USER user) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[user](USER userItem) { return _tcscmp(userItem.name, user.name) == 0 && _tcscmp(userItem.password, user.password) == 0; });
	if (it == userList.end())
		return false;
	else
		return true;
}

//TODO: check
/**
 * @brief Adiciona um utilizador ao servidor ou � fila de espera.
 *
 * @param servidor - Referencia do servidor.
 * @param user - Ponteiro para o utilizador a adicionar.
 * 
 * @return true se o utilizador foi adicionado ao servidor, false se foi adicionado � fila de espera.
 */
bool UserManager::addUser(BOLSA& servidor, USER* user) {
	if (servidor.hUsersList.size() < servidor.maxUsers) {
		servidor.hUsersList.push_back(user->hPipeInst);
		user->connected = true;
		return true;
	} else {
		servidor.hUsersQueue.push(user);
		user->inQueue = true;
		return false;
	}
}

//TODO: check
/**
 * @brief Remove um utilizador do servidor.
 *
 * @param userList - Lista de utilizadores.
 * @param userQueue - Fila de utilizadores.
 * @param user - Utilizador a remover.
 * 
 * @return Ponteiro para o utilizador que ficou em primeiro na fila de espera, ou nullptr se n�o houver utilizadores na fila.
 * 
 * @throw Se o utilizador n�o for encontrado.
 */
USER* UserManager::removeUser(USER_LIST& userList, USER_QUEUE& userQueue, USER* user) {
	auto it = std::find_if(userList.begin(), userList.end(),
				[user](USER userItem) { return _tcscmp(userItem.name, user->name) == 0; });

	if (it == userList.end()) {
		throw std::runtime_error("Cliente n�o encontrado"); //TODO: improve exception
	}

	it->connected = false;
	CloseHandle(it->hPipeInst);

	if (userQueue.empty())
		return nullptr;

	user = userQueue.front();
	user->connected = true;
	user->inQueue = false;
	userQueue.pop();

	return user;
}

/**
 * @brief Lista todos os utilizadores do servidor.
 *
 * @param servidor - Referencia do servidor.
 */
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
/**
 * @brief Devolve a refer�ncia do utilizador com o nome especificado.
 *
 * @param userList - Lista de utilizadores.
 * @param userName - Nome do utilizador a procurar.
 * 
 * @return Refer�ncia para o utilizador.
 * 
 * @throw Se o utilizador n�o for encontrado.
 */
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
/**
 * @brief Liberta todos os utilizadores do servidor.
 *
 * Esta fun��o deve ser chamada antes de terminar o servidor.
 *
 * @param servidor - Referencia do servidor.
 */
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