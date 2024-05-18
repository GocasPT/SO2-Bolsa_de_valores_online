#pragma once
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "board.h"
#include "SO2_Utils.h"
#include "SO2_SharedMemory.h"

namespace SharedMemory {
	void connect(BOARD& board);
	bool read(BOARD& board);
	void close(BOARD& board);
}
#endif // !SHARED_MEMORY_H