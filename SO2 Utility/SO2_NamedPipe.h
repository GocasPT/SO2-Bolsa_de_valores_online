#pragma once
#ifndef SO2_NAMED_PIPE_H
#define SO2_NAMED_PIPE_H

#include <tchar.h>

#define NAMED_PIPE_BOLSA_NAME _T("\\\\.\\pipe\\bolsa")
#define NAMED_PIPE_CLIENTE_NAME_FORMAT _T("\\\\.\\pipe\\cliente_%d")

#endif // !SO2_NAMED_PIPE_H