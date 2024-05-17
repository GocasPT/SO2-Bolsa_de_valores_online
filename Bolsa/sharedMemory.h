#pragma once
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "bolsa.h"
#include "SO2_SharedMemory.h"

namespace SharedMemory {
	void config(BOLSA &servidor);
	void update(BOLSA & servidor);
	void close(BOLSA& servidor);
}

#endif // !SHARED_MEMORY_H