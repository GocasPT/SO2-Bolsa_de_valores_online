#pragma once
#ifndef SO2_UTILS_H
#define SO2_UTILS_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include <sstream>
#include <iostream>
#include <array>

#ifdef UNICODE
	#define TSTRING wstring
	#define _tcout wcout
	#define _tcin wcin
	#define _tstringstream wstringstream
	#define _tifstream wifstream
	#define _tofstream wofstream
#else
	#define _tcout std::cout
	#define _tcin std::cin
	#define TSTRING std::string
	#define _tstringstream std::stringstream
	#define _tifstream std::ifstream
	#define _tofstream std::ofstream
#endif

#define MAX_TCHAR 256
#define MAX_STOCKS 10

typedef struct {
	TCHAR companyName[MAX_TCHAR];
	DWORD numStocks;
	float pricePerStock;
} STOCK_ITEM;

typedef struct {
	TCHAR name[MAX_TCHAR];
	TCHAR password[MAX_TCHAR];
	float balance;
	STOCK_ITEM wallet[MAX_STOCKS];
	DWORD walletSize;
	bool connected;
	bool inQueue;
} USER_DATA;

typedef struct {
	TCHAR name[MAX_TCHAR];
	DWORD numFreeStocks;
	float pricePerStock;
} COMPANY;

#endif // !SO2_UTILS_H
