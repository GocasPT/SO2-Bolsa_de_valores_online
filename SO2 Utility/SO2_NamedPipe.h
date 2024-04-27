#pragma once
#ifndef SO2_NAMED_PIPE_H
#define SO2_NAMED_PIPE_H

#include <tchar.h>

#define PIPE_TIMEOUT 1000
#define PIPE_BOLSA_NAME _T("\\\\.\\pipe\\bolsa")

//TODO: code for types of messages/requests (login, denid, listc, buy, sell, etc) [codes for server and cliente]
#define CODE_LOGIN 1
#define CODE_DENID -1
#define CODE_LISTC 2
#define CODE_LISTC_ITEM 3
#define CODE_BUY 4
#define CODE_SELL 5
#define CODE_BALANCE 6

#endif // !SO2_NAMED_PIPE_H