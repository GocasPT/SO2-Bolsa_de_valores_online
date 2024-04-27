#pragma once
#ifndef SO2_UTILS_H
#define SO2_UTILS_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

#ifdef UNICODE
	#define tcout wcout
	#define tcin wcin
	#define TSTRING wstring
	#define _tmain wmain
#else
	#define tcout std::cout
	#define tcin std::cin
	#define TSTRING std::string
	#define _tmain main
#endif

#define MAX 256

#define MUTEX_BOLSA_NAME _T("BOLSA_MUTEX")

typedef struct {
	//TODO: list of the stocks owned
	int PLACE_HOLDER;
} STOCK_WALLET;

typedef struct {
	TCHAR name[MAX];
	TCHAR password[MAX];
	float balance;
	STOCK_WALLET wallet; //TODO: check this
	bool connected;
} USER;

typedef struct {
	TCHAR name[MAX];
	DWORD numFreeStocks;
	float pricePerStock;
} COMPANY;

typedef struct {
	int numEnterprise;
	//TODO: list of N most value empresas
} SHARED_MEMORY;

typedef struct {
	TCHAR data[MAX];
} MESSAGE;

#endif // !SO2_UTILS_H
