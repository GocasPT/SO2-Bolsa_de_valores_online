#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include <vector>
#include <queue>

#define TAG_NORMAL _T("(BOLSA) ")
#define TAG_ERROR _T("[ERRO] ")
#define TAG_WARNING _T("[WARNING] ")

#define EVENT_CONSOLE _T("ServerConsole")
#define EVENT_TIMER _T("ServerTimer")

typedef struct USER : USER_DATA {
	HANDLE hPipeInst; // Handle of the cliente pipe
} USER;

typedef std::vector<USER> USER_LIST;
typedef std::queue<USER*> USER_QUEUE;
typedef std::vector<COMPANY> COMPANY_LIST;
typedef std::vector<HANDLE> HANDLE_LIST;

typedef struct {
	bool& isRunning;
	bool& isPaused;
	USER_LIST& userList;
	USER_QUEUE& userQueue;
	COMPANY_LIST& companyList;
	USER* myUser;
	CRITICAL_SECTION& cs;
} TDATA;

typedef struct {
	bool* isRunning;
	bool* isPaused;
	int time;
	HANDLE hEvent;
} TIMER_DATA;

typedef std::vector<TDATA> TDATA_LIST;

typedef struct {
	/* GENERAL */
	bool isRunning;
	bool isPaused;

	/* USER MANAGER */
	USER_LIST userList;
	DWORD maxUsers;

	/* COMPANY MANAGER */
	COMPANY_LIST companyList; // List of companies
	HANDLE hTimerThread; // Timer thread
	TIMER_DATA timerData; // Data for the timer

	/* SHARED MEMORY */
	HANDLE hSharedMemory; // Handle shared memory
	HANDLE hEvent; // Event to triggerto update the shared memory

	/* NAMED PIPE */
	HANDLE hPipeInst; // Current reciver pipe
	HANDLE hReciverThread; // Reciver thread
	HANDLE_LIST hUsersThreadList; // Thread for comunication with clients
	HANDLE_LIST hUsersList; // List of clients connected
	USER_QUEUE hUsersQueue; // Queue of clients waiting to connect
	TDATA_LIST tDataList; // Data for each thread
	CRITICAL_SECTION cs; // Critical section for shared data
} BOLSA;

#endif // !BOLSA_H