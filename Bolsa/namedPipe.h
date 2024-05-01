#pragma once
#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "bolsa.h"
#include "SO2_NamedPipe.h"

namespace NamedPipe {
	HANDLE newNamedPipe();
	void config(BOLSA& servidor);
	bool auth(BOLSA& servidor, USER &loginUser);
	DWORD WINAPI reciverRoutine(LPVOID lpParam);
	DWORD WINAPI userRoutine(LPVOID lpParam);
	void send(HANDLE hPipeInst, MESSAGE msg);
	void close(BOLSA& servidor);
}

#endif // !NAMED_PIPE_H