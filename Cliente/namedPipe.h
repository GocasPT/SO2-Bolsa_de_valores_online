#pragma once
#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "cliente.h"
#include "SO2_NamedPipe.h"

namespace NamedPipe {
	void connectToServer(CLIENTE& user);
	DWORD WINAPI reciverMessage(LPVOID lpParam);
	void send(CLIENTE& user, MESSAGE msg);
	void requestLogin(CLIENTE& user, std::TSTRING name, std::TSTRING password);
	void requestList(CLIENTE& user);
	void requestBuy(CLIENTE& user, std::TSTRING company, DWORD numOfStocks);
	void requestSell(CLIENTE& user, std::TSTRING company, DWORD numOfStocks);
	void requestBalance(CLIENTE& user);
	void close(CLIENTE &user);
}


#endif // !NAMED_PIPE_H