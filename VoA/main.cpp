#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <crtdbg.h>
#include "include/SDLWin.h"

int main ( int argc, char** argv )
{
	// Send all reports to STDOUT
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );

	SDLWin win;
    win.Run();
	
	int *test = new int[32];
	_CrtCheckMemory();
	_CrtDumpMemoryLeaks();
	
	return 0;
}
