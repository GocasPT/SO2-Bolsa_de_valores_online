#pragma once
#ifndef CLIENTE_H
#define CLIENTE_H

#include "SO2_Utils.h"

#define TAG_NORMAL _T("(CLIENTE) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

#define EVENT_CONSOLE _T("ClienteConsole")

typedef struct {
	OVERLAPPED oOverlap;
	HANDLE hPipe, hEvent;
} PIPE_INST;

typedef struct {
	bool logged;
	bool inQueue;
	TCHAR name[MAX_TCHAR];
	PIPE_INST hPipeInst;
	DWORD pipeMode;
	HANDLE hThread;

	HANDLE hEventConsole; // Evento para desbloquear a (thread do) console
	bool runnig;
} CLIENTE;

#endif // !CLIENTE_H
