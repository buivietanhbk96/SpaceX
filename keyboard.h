#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>

#define GROUP_KEY "aAwWsSdD qQ"
enum game_status
{
    PLAYING_STATUS,
    PAUSE_STAUS,
    GAMEOVER_STATUS
};
typedef struct keyQueue {
    struct keyQueue *next;
    char key;
} keyQueue_t;

void *read_keyboard(void *arg);

#endif