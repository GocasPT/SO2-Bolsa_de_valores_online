#include "userManager.h"
#include <algorithm>

/**
 * \brief Validar as credênciais do utilizador.
 *
 * \param userList - Lista de utilizadores.
 * \param user - Utilizador a validar.
 * 
 * \sreturn true se o utilizador for válido, false caso contrário.
 */
bool UserManager::validateUser(const USER_LIST& userList, USER user) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[user](USER userItem) { return _tcscmp(userItem.name, user.name) == 0 && _tcscmp(userItem.password, user.password) == 0; });
	if (it == userList.end())
		return false;
	else
		return true;
}

/**
 * \brief Adiciona um utilizador ao servidor ou à fila de espera.
 *
 * \param servidor - Referencia do servidor.
 * \param user - Ponteiro para o utilizador a adicionar.
 * 
 * \return true se o utilizador foi adicionado ao servidor, false se foi adicionado à fila de espera.
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

/**
 * \brief Remove um utilizador do servidor.
 *
 * \param userList - Lista de utilizadores.
 * \param userQueue - Fila de utilizadores.
 * \param user - Utilizador a remover.
 * 
 * \return Ponteiro para o utilizador que ficou em primeiro na fila de espera, ou nullptr se não houver utilizadores na fila.
 * 
 * \throw Se o utilizador não for encontrado.
 */
USER* UserManager::removeUser(USER_LIST& userList, USER_QUEUE& userQueue, USER* user) {
	auto it = std::find_if(userList.begin(), userList.end(),
				[user](USER userItem) { return _tcscmp(userItem.name, user->name) == 0; });

	if (it == userList.end()) {
		throw std::runtime_error("Cliente não encontrado"); //TODO: improve exception
	}

	it->connected = false;
	CloseHandle(it->hPipeInst.hPipe);

	if (userQueue.empty())
		return nullptr;

	user = userQueue.front();
	user->connected = true;
	user->inQueue = false;
	userQueue.pop();

	return user;
}

/**
 * \brief Lista todos os utilizadores do servidor.
 *
 * \param servidor - Referencia do servidor.
 */
void UserManager::listUsers(const BOLSA& servidor) {
	std::_tcout << _T("Users:") << std::endl;

	if (servidor.userList.empty()) {
		std::_tcout << _T("Nenhum cliente") << std::endl;
		return;
	}

	for (auto it = servidor.userList.begin(); it != servidor.userList.end(); it++)
		std::_tcout << _T("\tNome: ") << it->name <<
			_T(" | Saldo: ") << it->balance <<
			_T(" | Num de ações: ") << it->walletSize <<
			_T(" [") << (it->connected ? _T("Online") : it->inQueue ? _T("Fila de espera") : _T("Offline")) << _T("]")
			<< std::endl;

	std::_tcout << std::endl;
}

/**
 * \brief Devolve a referência do utilizador com o nome especificado.
 *
 * \param userList - Lista de utilizadores.
 * \param userName - Nome do utilizador a procurar.
 * 
 * \return Referência para o utilizador.
 * 
 * \throw Se o utilizador não for encontrado.
 */
USER& UserManager::getUser(USER_LIST& userList, std::TSTRING userName) {
	auto it = std::find_if(userList.begin(), userList.end(),
		[userName](USER user) { return userName.compare(user.name) == 0; });
	
	if (it == userList.end())
		throw std::runtime_error("User not found"); //TODO: improve exception
	else
		return *it;
}