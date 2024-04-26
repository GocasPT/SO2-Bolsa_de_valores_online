#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include <vector>
#include <queue>

#define TAG_NORMAL _T("(BOLSA) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

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
	HKEY hKey; //TODO: we need this?

	// User Manager
	std::vector<USER> userList;
	std::queue<USER> userQueue; //TODO: change and move this (queue of handles)
	DWORD maxUsers;

	// Company Manager
	std::vector<COMPANY> companyList;

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