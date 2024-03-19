#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>

#ifdef UNICODE 
#define tcout wcout
#define tcin wcin
#define tstring wstring
#else
#define tcout cout
#define tcin cin
#define tstring string
#endif

using namespace std;

int _tmain(int argc, tstring argv[]) {
	// VARIABLES

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif 

	tcout << TEXT("Sou o programa \'Board\'") << endl;

	return 0;
}