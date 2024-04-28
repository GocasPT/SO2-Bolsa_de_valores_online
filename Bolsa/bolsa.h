#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include <vector>
#include <queue>

#define TAG_NORMAL _T("(BOLSA) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

typedef std::vector<USER> USER_LIST;
typedef std::queue<USER> USER_QUEUE; //TODO: change and move this (queue of handles)
typedef std::vector<COMPANY> COMPANY_LIST;
typedef std::vector<HANDLE> HANDLE_LIST;

//TODO: add user info to know with one is "talking"
typedef struct {
	bool& isRunning;
	HANDLE hPipe;
	USER_LIST& userList;
	USER* myUser;
	CRITICAL_SECTION& cs;
} TDATA;

typedef std::vector<TDATA> TDATA_LIST;

//TODO: check if is missing any field
typedef struct {
	bool isRunning;

	// Registry
	HKEY hKey; //TODO: we need this?

	// User Manager
	USER_LIST userList;
	USER_QUEUE userQueue; //TODO: change and move this (queue of handles)
	DWORD maxUsers;

	// Company Manager
	std::vector<COMPANY> companyList;

	// Shared Memory
	HANDLE hSharedMemory;
	HANDLE hEvent;

	// Named Pipe
	HANDLE hPipe;
	HANDLE hReciverThread;
	HANDLE_LIST hUsersThreadList;
	HANDLE_LIST hUsersPipesList;
	TDATA_LIST tDataList;
	CRITICAL_SECTION cs;
} BOLSA;

#endif // !BOLSA_H