#ifndef _BULLET_H_
#define _BULLET_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "keyboard.h"
#include "utilities.h"
#include "ship.h"

#define BULLET_CHAR   "\ua537"
/* #define BULLET_CHAR   "*" */
#define BULLET_SPEED 20000
typedef struct {
	int sx;
	int sy;
} Bullet_t;

static void _clean_old_bullet(Bullet_t *bullet);
static void _draw_bullet(Bullet_t *bullet);
Bullet_t *create_bullet(int x, int y);
void *handle_bullet(void *arg);
#endif