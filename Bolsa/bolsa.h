#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include <vector>
#include <queue>

typedef struct {
	BOOL isRunning;
	std::vector<USER>* userList;
	std::queue<USER>* userQueue;
	DWORD maxUsers;
	CRITICAL_SECTION* cs;
} TDATA;

//TODO: check if is missing any field
typedef struct {
	// Registry
	HKEY hKey;

	// User Manager
	std::vector<USER> userList;
	std::queue<USER> userQueue;
	DWORD maxUsers;

	// Shared Memory
	HANDLE hSharedMemory;
	HANDLE hEvent;

	// Named Pipe
	HANDLE hPipe;
	HANDLE hReciverThread;
	std::vector<HANDLE> hUsersThreadList;
	std::vector<HANDLE> hUsersPipesList;
	CRITICAL_SECTION cs;
	TDATA tData;
} BOLSA;

#endif // !BOLSA_H