#include "utilities.h"


void gotoxy(int x, int y)
{
    printf("\033[%d;%df", x, y);
}

void get_window_size(int *line, int *column)
{
    struct winsize ws;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);

	*line  =  ws.ws_row;
	*column =  ws.ws_col;
    return;
}