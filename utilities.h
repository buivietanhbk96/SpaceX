#ifndef _UTILITIES_H_
#define _UTILITIES_H_
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
void gotoxy(int x, int y);
void get_window_size(int *line, int *column);
#endif