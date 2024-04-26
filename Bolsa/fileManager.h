#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "bolsa.h"

#define TOTAL_FILES 2
#define FILE_USERS _T("users.txt")
#define FILE_COMPANIES _T("companies.txt")
//TODO: maybe add more files to save

namespace Files {
	void read_files(BOLSA& servidor);
	void write_files(BOLSA& servidor);
}

#endif // !FILE_MANAGER_H
