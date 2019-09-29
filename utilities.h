#ifndef _UTILITIES_H_
#define _UTILITIES_H_
#include <stdio.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>


#define MAX_ROW 60
#define MAX_COLUMN 210
enum map_object_t{ EMPTY, BULLET, SHIP, METEOR };
void gotoxy(int x, int y);
void get_window_size(int *line, int *column);

int check_coordinate_bullet(int x, int y);
int check_coordinate_meteor(int x, int y, int length);
#endif