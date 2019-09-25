#include "ship.h"
pthread_mutex_t ship_mutex;
const char ship_x[4][15] ={"   /^\\"," |/| |\\|","|       |"," *-\"\"\"-*"};
const char ship_del[4][15] ={"      ","        ","         ","        "};
static int win_line;
static int win_col;
static void _clean_draw_ship(Spaceship_t *ship);
Spaceship_t *init_spaceship(void)
{
    Spaceship_t *spaceship = (Spaceship_t *)malloc(sizeof(Spaceship_t));
    get_window_size(&win_line, &win_col);
    if(NULL == spaceship)
    {
        printf("Out of memory when mallocing\n");
        return NULL;
    }
    else
    {
        spaceship->sx = win_line - 4;     /* 4 because using array [4][15] to store char of ship */
        spaceship->sy = win_col / 2;
        spaceship->lives = 3;
        spaceship->bullet_type = 1;
    }
    return spaceship;
}
/*
* delete old char when moving the ship
*/
static void _clean_draw_ship(Spaceship_t *ship)
{
    int i;
    pthread_mutex_lock(&ship_mutex);
    for(i = 0; i < 4; i++)
    {
        gotoxy((ship->sx) + i,ship->sy);
        printf("%s\n",ship_del[i]);
    }
    pthread_mutex_unlock(&ship_mutex);
}

void draw_spaceship(Spaceship_t *ship)
{   
    int i;
    pthread_mutex_lock(&ship_mutex);
    for(i = 0; i < 4; i++)
    {
        gotoxy((ship->sx) + i,ship->sy);
        printf("%s\n",ship_x[i]);
    }
    pthread_mutex_unlock(&ship_mutex);
}

void move_spaceship(Spaceship_t *ship, enum direction_t direct)
{
     
    //_clean_draw_ship(ship);
    if(LEFT == direct)
    {
        if(ship->sy >= SHIP_JUMP)
        {
            _clean_draw_ship(ship);
            ship->sy -= SHIP_JUMP;
            draw_spaceship(ship);
        }
    }
    else if(RIGHT == direct)
    {
        if(ship->sy <= (win_col - SHIP_JUMP - 10))    /* 10 because length of max string of the ship */
        {
            _clean_draw_ship(ship);
            ship->sy += SHIP_JUMP;
            draw_spaceship(ship);
        }
    }
    else if(UP == direct)
    {
        if(ship->sx >= SHIP_JUMP)
        {
            _clean_draw_ship(ship);
            ship->sx -= SHIP_JUMP;
            draw_spaceship(ship);
        }
    }
    else if(DOWN == direct)
    {
        if(ship->sx <= (win_line - SHIP_JUMP - 4))
        {
            _clean_draw_ship(ship);
            ship->sx += SHIP_JUMP;
            draw_spaceship(ship);
        }
    }
}