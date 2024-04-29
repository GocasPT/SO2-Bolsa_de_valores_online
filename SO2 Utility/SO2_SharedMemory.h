#pragma once
#ifndef SO2_SHARED_MEMORY_H
#define SO2_SHARED_MEMORY_H

#include <tchar.h>

#define EVENT_NAME _T("SHARED_MEMORY_EVENT")

typedef struct {
	int numEnterprise;
	//TODO: list of N most value empresas
} SHARED_MEMORY;

#endif // !SO2_SHARED_MEMORY_H
