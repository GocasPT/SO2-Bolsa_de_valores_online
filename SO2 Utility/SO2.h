#pragma once

#include <windows.h>
#include <string>

#ifdef UNICODE
	#define tcout wcout
	#define tcin wcin
	#define TSTRING wstring
	#define _tmain wmain
#else
	#define tcout cout
	#define tcin cin
	#define TSTRING string
	#define _tmain main
#endif

typedef struct {
	//TODO: list of the stocks owned
	int PLACE_HOLDER;
} STOCK_WALLET;

typedef struct {
	std::TSTRING name;
	std::TSTRING password;
	DWORD balance;
	STOCK_WALLET wallet; //TODO: check this
} USER;

typedef struct {
	std::TSTRING name;
	DWORD numFreeStocks;
	DWORD pricePerStock;
} COMPANY;

typedef struct {
	int numEnterprise;
	//TODO: list of N most value empresas
} SHARED_MEMORY;

typedef struct {
	//TODO
	std::TSTRING name;
} REQUEST_COMMAND;

typedef struct {
	//TODO
	std::TSTRING message;
} FEEDBACK_SERVER;