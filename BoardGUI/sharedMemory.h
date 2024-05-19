#pragma once
#pragma once
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "boardGUI.h"

namespace SharedMemory {
	void connect(BOARD& board);
	bool read(BOARD& board);
	void close(BOARD& board);
}
#endif // !SHARED_MEMORY_H