#pragma once
#ifndef CLIENTE_H
#define CLIENTE_H

#include "SO2_Utils.h"

#define TAG_NORMAL _T("(CLIENTE) ")

#define EVENT_CONSOLE _T("ClienteConsole")

typedef struct {
	OVERLAPPED oOverlap;
	HANDLE hPipe, hEvent;
} PIPE_INST;

typedef struct {
	bool runnig;
	bool logged;
	bool inQueue;
	TCHAR name[MAX_TCHAR];
	PIPE_INST hPipeInst;
	DWORD pipeMode;
	HANDLE hThread;
	HANDLE hEventConsole; // Evento para desbloquear a console
} CLIENTE;

#endif // !CLIENTE_H
