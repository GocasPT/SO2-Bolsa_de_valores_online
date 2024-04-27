#pragma once
#ifndef CLIENTE_H
#define CLIENTE_H

#include "SO2_Utils.h"

#define TAG_NORMAL _T("(CLIENTE) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

typedef struct {
	TCHAR name[MAX];
	TCHAR password[MAX];
	float balance;
	HANDLE hPipe;
	DWORD pipeMode;
	HANDLE hThread;
	bool tContinue;
} CLIENTE;

#endif // !CLIENTE_H
