#pragma once
#ifndef SO2_SHARED_MEMORY_H
#define SO2_SHARED_MEMORY_H

#include <tchar.h>

#define EVENT_NAME _T("SHARED_MEMORY_EVENT")
#define SHARED_MEMORY_NAME _T("SHARED_MEMORY")

#define MAX_COMPANIES 30

typedef struct {
	DWORD numCompanies;
	COMPANY companies[MAX_COMPANIES];
	TRANSACTION lastTransaction;

	DWORD numBoards;
	DWORD boardsRead;
} SHARED_MEMORY;

#endif // !SO2_SHARED_MEMORY_H
