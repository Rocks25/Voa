#include "include/SDLWin.h"


int main ( int argc, char** argv )
{
	printf("%s\n",glGetString(GL_EXTENSIONS));
    SDLWin win;
    win.Run();

    return 0;
}
