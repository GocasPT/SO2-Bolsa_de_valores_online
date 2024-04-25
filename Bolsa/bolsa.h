#pragma once
#ifndef BOLSA_H
#define BOLSA_H

#include "SO2_Utils.h"
#include <vector>
#include <queue>

//TODO: check if is missing any field
typedef struct {
	std::vector<USER> userList;
	std::queue<USER> userQueue;
	DWORD maxUsers;
} BOLSA;

#endif // !BOLSA_H