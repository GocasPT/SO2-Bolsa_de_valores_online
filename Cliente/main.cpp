#include "SO2.h"
#include "named pipe.h"
#include <fcntl.h>
#include <io.h>
#include <iostream>

int _tmain(int argc, std::TSTRING argv[]) {
	// VARIABLES

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	std::tcout << TEXT("Sou o programa \'Cliente\'") << std::endl;
	std::tcin;

	return 0;
}