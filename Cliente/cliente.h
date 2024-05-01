#pragma once
#ifndef CLIENTE_H
#define CLIENTE_H

#include "SO2_Utils.h"

#define TAG_NORMAL _T("(CLIENTE) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

typedef struct {
	HANDLE hPipe;
	OVERLAPPED overlap;
	HANDLE hEvent;
} PIPE_INST;

typedef struct {
	bool logged;
	bool inQueue;
	TCHAR name[MAX];
	PIPE_INST hPipeInst;
	DWORD pipeMode;
	HANDLE hThread;
	bool runnig;
} CLIENTE;

#endif // !CLIENTE_H
