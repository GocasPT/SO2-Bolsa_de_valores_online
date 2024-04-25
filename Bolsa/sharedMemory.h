#pragma once
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "SO2_Utils.h"
#include "SO2_SharedMemory.h"
#include "bolsa.h"
#include <vector>

//TODO: functions for shared memory (server-side)
void updateSharedMemory(BOLSA &bolsa);

#endif // !SHARED_MEMORY_H