//#include "SO2.h"
#include "named pipe.h"
#include "UI.h"
#include <fcntl.h>
#include <io.h>

int _tmain(int argc, TSTRING argv[]) {
	// VARIABLES

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	tcout << TEXT("Sou o programa \'Cliente\'") << std::endl;
	tcin;

	return 0;
}