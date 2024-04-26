#pragma once
#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "bolsa.h"
#include "SO2_NamedPipe.h"

#define PIPE_TIMEOUT 1000

namespace NamedPipe {
	//TODO: Functions for named  pipe (server-side)
	void config(BOLSA& servidor);
	DWORD WINAPI reciverRoutine(LPVOID lpParam);
	void send(BOLSA& servidor, std::TSTRING msg);
	void close(BOLSA& servidor);
}

#endif // !NAMED_PIPE_H