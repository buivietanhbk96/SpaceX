#ifndef _METEORITE_H_
#define _METEORITE_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "utilities.h"
#include "score.h"

#define METEO_CHAR_SMALL "\u2588\u2588"
#define METEO_CHAR_MEDIUM "\u2588\u2588\u2588\u2588\u2588"
#define METEO_CHAR_LARGE "\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588"

#define SPEED_METEORITE 500000
enum meteo_scale_t {SMALL, MEDIUM, LARGE};
typedef struct 
{
    int sx;
    int sy;
    enum meteo_scale_t scale;
    int health;
    int length;
    int height;
    int speed;
}Meteorite_t;

void draw_meteorite(Meteorite_t * meteor);
Meteorite_t *create_meteorite(void);

void *handle_meteorite(void *arg);

#endif