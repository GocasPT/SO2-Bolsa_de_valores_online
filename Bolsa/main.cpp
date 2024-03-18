#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <fcntl.h> 
#include <windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	// VARIABLES

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	_tprintf(TEXT("Sou o programa \'Bolsa\'"));

	return 0;
}