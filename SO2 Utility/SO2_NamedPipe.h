#pragma once
#ifndef SO2_NAMED_PIPE_H
#define SO2_NAMED_PIPE_H

#include <tchar.h>

#define PIPE_TIMEOUT 1000
#define PIPE_BOLSA_NAME _T("\\\\.\\pipe\\bolsa")

enum MESSAGE_CODE {
	CODE_LOGIN,
	CODE_DENID,
	CODE_FULL,
	CODE_LISTC,
	CODE_LISTC_ITEM,
	CODE_BUY,
	CODE_SELL,
	CODE_BALANCE,
	CODE_EXIT,
	CODE_GENERIC_FEEDBACK,
	CODE_NOTIFY
};

typedef struct {
	DWORD code;
	TCHAR data[MAX_TCHAR];
} MESSAGE;

#endif // !SO2_NAMED_PIPE_H