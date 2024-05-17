#include "registry.h"

void Registry::config(BOLSA& servidor) {
	std::_tcout << _T("A buscar os dados ao Registry...") << std::endl;

	HKEY hKey;
	DWORD disposition;

	RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_PATH, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disposition);
	if (disposition == REG_CREATED_NEW_KEY) {
		std::_tcout << _T("Criado o ficheiro no registry com o valor default [ MAX USERS: ") << DEFAULT_MAX_USERS << _T("]") << std::endl;
		servidor.maxUsers = DEFAULT_MAX_USERS;
		RegSetValueEx(hKey, VALUE_MAX_USERS, 0, REG_DWORD, (LPBYTE)&servidor.maxUsers, sizeof(servidor.maxUsers));
	} else {
		DWORD dwSize = sizeof(servidor.maxUsers);
		RegQueryValueEx(hKey, VALUE_MAX_USERS, NULL, NULL, (LPBYTE)&servidor.maxUsers, &dwSize);
		std::_tcout << _T("Leitura do valor no registry [ MAX USERS: ") << servidor.maxUsers << _T("]") << std::endl;
	}

	servidor.maxUsers = 1;

	std::_tcout << _T("Dados lidos com sucesso") << std::endl << std::endl;
}