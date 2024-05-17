#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include "SO2_SharedMemory.h"
#include <vector>
#include <list>
#include <queue>

#define TAG_NORMAL _T("(BOLSA) ")

#define EVENT_CONSOLE _T("ServerConsole")
#define EVENT_DATA _T("ServerData")

typedef struct {
	OVERLAPPED oOverlap;			// Overlapped structure for async I/O
	OVERLAPPED oOverlapExtra;		// Overlapped structure for extra async I/O
	HANDLE hPipe;					// Handle of the pipe
	HANDLE hEvent;			// Event handle
	HANDLE hEventExtra;			// Extra event handle
} PIPE_INST;

typedef struct USER : USER_DATA {
	PIPE_INST hPipeInst;			// Handle of the cliente pipe
} USER;

typedef struct {
	COMPANY* company;				// Ponteiro para a empresa com última alteração
	float oldPrice;					// Preço antigo (preço atual já alterado)
	USER_DATA user;
} NOTIFY_DATA;

typedef std::vector<USER> USER_LIST;
typedef std::queue<USER*> USER_QUEUE;
typedef std::vector<COMPANY> COMPANY_LIST;
typedef std::vector<HANDLE> HANDLE_LIST;
typedef std::vector<PIPE_INST> PIPE_INST_LIST;

typedef struct {
	/*--REFERENCIAS DO ERVIDOR---*/
	bool& isRunning;				// Referencia para o estado do servidor
	bool& isPaused;					// Referencia para o estado do servidor
	NOTIFY_DATA& notifyData;		// Referencia para os dados de notificação

	/*---LISTAS DO SERVIDOR---*/
	USER_LIST& userList;			// Lista de utilizadores
	USER_QUEUE& userQueue;			// Fila de utilizadores
	COMPANY_LIST& companyList;		// Lista de empresas

	/*---DADOS DA THREAD---*/
	DWORD tID;						// Thread ID
	USER* myUser; 					// Utilizador associado à thread

	/*---CRITICAL SECTION---*/
	CRITICAL_SECTION& cs;			// Critical section
} TDATA;

typedef struct {
	bool* isRunning;				// Referencia para o estado do servidor
	bool* isPaused;					// Referencia para o estado do servidor
	int time;						// Tempo do timer
	HANDLE hEvent;					// Evento para o timer
} TIMER_DATA;

typedef std::list<TDATA> TDATA_LIST;

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

	/*---GESTOR DE DADOS---*/
	HANDLE hDataThread;				// Data thread
	HANDLE hDataEvent;				// Event to triggerto update the data

	/* SHARED MEMORY */
	HANDLE hSharedMemory;			// Handle shared memory
	HANDLE hEvent;					// Event to triggerto update the shared memory
	SHARED_MEMORY* sharedMemory;

	/*---NAMED PIPE---*/
	PIPE_INST hPipeInst;			// Current receiver pipe
	HANDLE hReceiverThread;			// Receiver thread

	/*---NOTIFY---*/
	HANDLE hNotifyThread;			// Notify thread
	HANDLE hNotifyEvent;			// Event to triggerto notify the clients
	NOTIFY_DATA notifyData;			// Data for the notify

	HANDLE_LIST hUsersThreadList;	// Threads for comunication with clients
	PIPE_INST_LIST hUsersList;		// List of clients connected
	USER_QUEUE hUsersQueue;			// Queue of clients waiting to connect
	TDATA_LIST tDataList;			// Data for each thread
	CRITICAL_SECTION cs;			// Critical section for shared data

	HANDLE hExitEvent;
} BOLSA;

#endif // !BOLSA_H