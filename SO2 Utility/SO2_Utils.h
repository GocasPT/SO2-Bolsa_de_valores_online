#pragma once
#ifndef SO2_UTILS_H
#define SO2_UTILS_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <iostream>

#ifdef UNICODE
	#define TSTRING wstring
	#define _tcout wcout
	#define _tcin wcin
	#define _tstringstream wstringstream
	#define _tifstream wifstream
#else
	#define _tcout std::cout
	#define _tcin std::cin
	#define TSTRING std::string
	#define _tstringstream std::stringstream
	#define _tifstream std::ifstream
#endif

#define MAX 256

#define MUTEX_BOLSA_NAME _T("BOLSA_MUTEX")

typedef struct {
	//TODO: list of the stocks owned
	int PLACE_HOLDER;
} STOCK_WALLET;

//TODO: change some fields
typedef struct {
	TCHAR name[MAX];
	TCHAR password[MAX];
	float balance;
	STOCK_WALLET wallet; //TODO: check this
	bool connected;
	bool inQueue;
} USER_DATA;

typedef struct {
	TCHAR name[MAX];
	DWORD numFreeStocks;
	float pricePerStock;
} COMPANY;

#endif // !SO2_UTILS_H
