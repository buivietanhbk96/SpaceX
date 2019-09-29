#ifndef _SCORE_H_
#define _SCORE_H_

#include <stdio.h>
#include "ship.h"
#include "meteorite.h"

#define SMALL_SCORE   100
#define MEDIUM_SCORE  200
#define LARGE_SCORE   300

#define LIVE_CHAR "\u2661"
void add_score(int scale);
int get_score();
void lose_one_life();
void draw_score();
#endif