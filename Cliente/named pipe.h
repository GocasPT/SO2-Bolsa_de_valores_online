#pragma once
#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "cliente.h"
#include "SO2_NamedPipe.h"

namespace NamedPipe {
	void connectToServer(CLIENTE& user);
	DWORD WINAPI reciverMessage(LPVOID lpParam);
	void send(CLIENTE& user, std::TSTRING msg);
}


#endif // !NAMED_PIPE_H