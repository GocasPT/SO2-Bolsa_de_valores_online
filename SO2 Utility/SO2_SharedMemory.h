#pragma once
#ifndef SO2_SHARED_MEMORY_H
#define SO2_SHARED_MEMORY_H

#include <tchar.h>

#define EVENT_NAME _T("SHARED_MEMORY_EVENT")
#define SHARED_MEMORY_NAME _T("SHARED_MEMORY")

#define N 10

typedef struct {
	DWORD numCompanies;
	COMPANY companies[N];
	TRANSACTION lastTransaction;
} SHARED_MEMORY;

#endif // !SO2_SHARED_MEMORY_H
