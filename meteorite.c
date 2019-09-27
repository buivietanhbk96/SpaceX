#include "meteorite.h"

extern pthread_mutex_t draw_mutex;
extern int map_arr[MAX_ROW][MAX_COLUMN];
extern pthread_mutex_t access_map_arr_mutex;
extern int win_col;
extern int win_line;
static void _clear_meteor(Meteorite_t *meteor);
static void _update_position_meteor(Meteorite_t *meteor, int value)
{
    int i, length;
    switch (meteor->scale)
    {
    case SMALL:
        length = 2;
        break;
    case MEDIUM:
        length = 5;
        break;
    case LARGE:
        length = 10;
        break;
    }
    pthread_mutex_lock(&access_map_arr_mutex);
    for(i = 0; i < length; i++)
    {
        map_arr[meteor->sx][meteor->sy + i] = value;
    }
    pthread_mutex_unlock(&access_map_arr_mutex);
}

void draw_meteorite(Meteorite_t *meteor)
{
    pthread_mutex_lock(&draw_mutex);
    gotoxy(meteor->sx, meteor->sy);
    switch (meteor->scale)
    {
    case SMALL:
        printf(METEO_CHAR_SMALL);
        break;
    case MEDIUM:
        printf(METEO_CHAR_MEDIUM);
        break;
    case LARGE:
        printf(METEO_CHAR_LARGE);
        break;            
    }
    
    pthread_mutex_unlock(&draw_mutex);
    _update_position_meteor(meteor,METEOR);
    meteor->sx +=1;
}
static void _clear_meteor(Meteorite_t *meteor)
{
    pthread_mutex_lock(&draw_mutex);
    switch (meteor->scale)
    {
    case SMALL:
        gotoxy(meteor->sx  - 2,meteor->sy);
        printf("  ");
        break;
    case MEDIUM:
        gotoxy(meteor->sx  - 3,meteor->sy);
        printf("     ");
        break;
    case LARGE:
        gotoxy(meteor->sx  - 5,meteor->sy);
        printf("          ");
        break;
    }
    pthread_mutex_unlock(&draw_mutex);
    _update_position_meteor(meteor,EMPTY);
}
/* Ham nay co chuc nang tao ra 1 thien thach voi kich thuoc va vi tri ngau nhien tren top screen */
Meteorite_t *create_meteorite(void)
{
    
    Meteorite_t *meteor = (Meteorite_t *)malloc(sizeof(Meteorite_t));
    meteor->sx = 0;
    meteor->scale = rand() % 3;
    switch (meteor->scale)
    {
    case SMALL:
        meteor->sy = rand() % (win_col - 2);    /* 2 is length of small string meteor */
        meteor->meteo_interval = 1;
        meteor->health = 15;
        meteor->length = 2;
        break;
    case MEDIUM:
        meteor->sy = rand() % (win_col - 5);    /* 5 is length of medium string meteor */
        meteor->meteo_interval = 2;
        meteor->health = 30;
        meteor->length = 5;
        break;
    case LARGE:
        meteor->sy = rand() % (win_col - 10);   /* 10 is length of large string meteor */
        meteor->meteo_interval = 4;
        meteor->health = 60;
        meteor->length = 10;
        break;
    }
    meteor->speed = 1;
    return meteor;
}

void *handle_meteorite(void *arg)
{
    int count = 0, rt_check;
    Meteorite_t *meteorx = create_meteorite();
    while(1)
    {
        rt_check = check_coordinate_meteor(meteorx->sx, meteorx->sy, meteorx->length);
        if(BULLET == rt_check)
        {
            meteorx->health -= 5;
            if(0 == meteorx->health)
            {
                printf("kill the meteor");
                break;
            }
                
        }
        else if(SHIP == rt_check)
        {
            /* the live of ship -= 1*/
            break;
        }
        else
        {
            draw_meteorite(meteorx);
            count++;
            if (count > (meteorx->meteo_interval))
            {
                _clear_meteor(meteorx);
            }
            usleep(SPEED_METEORITE);
            if (meteorx->sx > win_line + 4)
            {
                break;
            }
        }
    }
    printf("kill the meteor");
    free(meteorx);
    pthread_exit(NULL);
}