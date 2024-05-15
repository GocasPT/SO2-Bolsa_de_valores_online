#pragma once
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "bolsa.h"
#include <Windows.h>
#include "SO2_SharedMemory.h"

namespace SharedMemory {
	//TODO: functions for shared memory (server-side)
	void config(BOLSA &servidor);
	void update(BOLSA & servidor);
	void close(BOLSA& servidor);
}

#endif // !SHARED_MEMORY_H