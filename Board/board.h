#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "SO2_Utils.h"
#include "SO2_SharedMemory.h"
#include <vector>
#include <queue>

#define TAG_NORMAL _T("(BOARD) ")

typedef struct {
	/* GENERAL */
	bool isRunning;
	bool isPaused;
	DWORD N;

	/* SHARED MEMORY */
	HANDLE hSharedMemory; // Handle shared memory
	HANDLE hEvent; // Event to trigger update on the shared memory
	SHARED_MEMORY* sharedMemory;

	HANDLE hConsoleThread;

	HANDLE hExitEvent;

	SHARED_MEMORY data;
} BOARD;

#endif // !BOARD_H