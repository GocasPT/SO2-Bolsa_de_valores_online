#pragma once
#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "SO2_Utils.h"
#include "SO2_NamedPipe.h"
#include "bolsa.h"

DWORD WINAPI clienteReceiver(LPVOID lpParam);
void sendMessage(BOLSA &bolsa, std::TSTRING msg);

//TODO: Functions for named  pipe (server-side)

#endif // !NAMED_PIPE_H