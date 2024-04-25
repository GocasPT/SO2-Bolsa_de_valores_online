#pragma once
#ifndef UI

#include "SO2 Utils.h"

#define TAG_INPUT _T(">>")
#define TAG_OUTPUT_CLIENTE _T("[CLIENTE]")
#define TAG_OUTPUT_BOLSA _T("[BOLSA]")

BOOL validateCommand(const TSTRING& input);
DWORD WINAPI console(LPVOID lpParam);

#endif // !UI
