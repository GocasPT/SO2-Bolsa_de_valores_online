#pragma once
#ifndef BOARD_GUI_H
#define BOARD_GUI_H

#include "SO2_Utils.h"
#include "SO2_SharedMemory.h"

typedef struct {
	/* GENERAL */
	bool isRunning;
	bool isPaused;

	/* SHARED MEMORY */
	HANDLE hSharedMemory; // Handle shared memory
	HANDLE hEvent; // Event to trigger update on the shared memory
	SHARED_MEMORY* sharedMemory;

	HANDLE hExitEvent;

	SHARED_MEMORY data;
} BOARD;

#endif // !BOARD_GUI_H