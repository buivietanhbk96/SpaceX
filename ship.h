#ifndef _SHIP_H_
#define _SHIP_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "utilities.h"

#define SHIP_JUMP  2
#define SHIP_MAX_SPEED  2.0f 

enum direction_t { LEFT, RIGHT, UP, DOWN };
typedef struct {
	int sx;
	int sy;
	int lives;
    int bullet_type;
} Spaceship_t;



Spaceship_t *init_spaceship(void);
void draw_spaceship(Spaceship_t *ship);
void move_spaceship(Spaceship_t *ship, enum direction_t direct);
void fire_spaceship(Spaceship_t *ship);
int get_coordinate_ship(int *x, int *y);
#endif