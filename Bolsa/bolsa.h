#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include <vector>
#include <queue>

#define TAG_NORMAL _T("(BOLSA) ")

#define EVENT_CONSOLE _T("ServerConsole")
#define EVENT_TIMER _T("ServerTimer")

typedef struct {
	OVERLAPPED oOverlap;
	HANDLE hPipe, hEvent;
} PIPE_INST;

typedef struct USER : USER_DATA {
	PIPE_INST hPipeInst;			// Handle of the cliente pipe
} USER;

typedef std::vector<USER> USER_LIST;
typedef std::queue<USER*> USER_QUEUE;
typedef std::vector<COMPANY> COMPANY_LIST;
typedef std::vector<HANDLE> HANDLE_LIST;
typedef std::vector<PIPE_INST> PIPE_INST_LIST;

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
	/*---GENERAL---*/
	bool isRunning;
	bool isPaused;

	/*---USER MANAGER---*/
	USER_LIST userList;
	DWORD maxUsers;

	/*---COMPANY MANAGER---*/
	COMPANY_LIST companyList;		// List of companies
	HANDLE hTimerThread;			// Timer thread
	TIMER_DATA timerData;			// Data for the timer

	/*---SHARED MEMORY---*/
	HANDLE hSharedMemory;			// Handle shared memory
	HANDLE hEvent;					// Event to triggerto update the shared memory

	/*---GESTOR DE DADOS*/
	HANDLE hDataThread;				// Data thread

	/*---NAMED PIPE---*/
	PIPE_INST hPipeInst;			// Current reciver pipe
	HANDLE hReciverThread;			// Reciver thread
	HANDLE hNotifyThread;			// Notify thread
	HANDLE_LIST hUsersThreadList;	// Threads for comunication with clients
	PIPE_INST_LIST hUsersList;		// List of clients connected
	USER_QUEUE hUsersQueue;			// Queue of clients waiting to connect
	TDATA_LIST tDataList;			// Data for each thread
	CRITICAL_SECTION cs;			// Critical section for shared data
} BOLSA;

#endif // !BOLSA_H