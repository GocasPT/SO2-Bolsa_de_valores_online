#pragma once
#ifndef UI

#include "cliente.h"

#define TAG_INPUT _T(">> ")
#define TAG_OUTPUT_BOLSA _T("<BOLSA> ")

namespace UI {
	BOOL validateCommand(CLIENTE userData, const std::TSTRING& input);
	DWORD WINAPI console(LPVOID lpParam);
}

#endif // !UI
