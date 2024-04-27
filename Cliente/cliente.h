#pragma once
#ifndef CLIENTE_H
#define CLIENTE_H

#include "SO2_Utils.h"

#define TAG_NORMAL _T("(CLIENTE) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

typedef struct {
	bool logged;
	TCHAR name[MAX];
	TCHAR password[MAX];
	float balance; //TODO: maybe remove this (just request to server)
	HANDLE hPipe;
	DWORD pipeMode;
	HANDLE hThread;
	bool tContinue; //TODO: change name
} CLIENTE;

#endif // !CLIENTE_H
