#include "SO2_Utils.h"
#include "namedPipe.h"
#include "sharedMemory.h"
#include "bolsa.h"
#include <fcntl.h>
#include <io.h>

int _tmain(int argc, std::TSTRING argv[]) {
	// VARIABLES

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	std::tcout << TEXT("Sou o programa \'Bolsa\'") << std::endl;
	std::tcin;

	return 0;
}