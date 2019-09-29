#include "utilities.h"

extern int map_arr[MAX_ROW][MAX_COLUMN];
extern pthread_mutex_t access_map_arr_mutex;
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
}

int check_coordinate_bullet(int x, int y)
{
    int result;
    pthread_mutex_lock(&access_map_arr_mutex);
    result = map_arr[x][y];
    pthread_mutex_unlock(&access_map_arr_mutex);
    return result;
}

int check_coordinate_meteor(int x, int y, int length)
{
    int i, result = 0;
    pthread_mutex_lock(&access_map_arr_mutex);

    for (i = 0; i < length; i++)
    {
        if (0 != map_arr[x][y + i])
        {
            result = map_arr[x][y + i];
            break;
        }
    }
    pthread_mutex_unlock(&access_map_arr_mutex);
    return result;
}