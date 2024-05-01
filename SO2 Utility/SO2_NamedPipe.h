#pragma once
#ifndef SO2_NAMED_PIPE_H
#define SO2_NAMED_PIPE_H

#include <tchar.h>

#define PIPE_TIMEOUT 1000
#define PIPE_BOLSA_NAME _T("\\\\.\\pipe\\bolsa")

//TODO: code for types of messages/requests (login, denid, listc, buy, sell, etc) [codes for server and cliente]
//TODO: set better codes
#define CODE_LOGIN 1
#define CODE_DENID 8
#define CODE_FULL 0
#define CODE_LISTC 2
#define CODE_LISTC_ITEM 3
#define CODE_BUY 4
#define CODE_SELL 5
#define CODE_BALANCE 6
#define CODE_GENERIC_FEEDBACK 7

typedef struct {
	DWORD code;
	TCHAR data[MAX];
} MESSAGE;

#endif // !SO2_NAMED_PIPE_H