#include "meteorite.h"

extern pthread_mutex_t draw_mutex;
extern int map_arr[MAX_ROW][MAX_COLUMN];
extern pthread_mutex_t access_map_arr_mutex;
extern int win_col;
extern int win_line;
extern int game_status;
static void _clear_meteor(Meteorite_t *meteor);
static void _update_position_meteor(Meteorite_t *meteor, int value)
{
    int i;
    pthread_mutex_lock(&access_map_arr_mutex);
    if (EMPTY == value)
    {
        if((meteor->sx - meteor->height - 1) >= 0)
        {
            for (i = 0; i < meteor->length; i++)
            {
                map_arr[meteor->sx - meteor->height - 1][meteor->sy + i] = value;
            }
        }
    }
    else
    {
        for (i = 0; i < meteor->length; i++)
        {
            map_arr[meteor->sx][meteor->sy + i] = value;
        }
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
    if(meteor->sx > meteor->height)
    {
        switch (meteor->scale)
        {
        case SMALL:
            gotoxy(meteor->sx - 2, meteor->sy);
            printf("  ");
            break;
        case MEDIUM:
            gotoxy(meteor->sx - 3, meteor->sy);
            printf("     ");
            break;
        case LARGE:
            gotoxy(meteor->sx - 5, meteor->sy);
            printf("          ");
            break;
        }
    }
    pthread_mutex_unlock(&draw_mutex);
    _update_position_meteor(meteor,EMPTY);
}
static void _destroy_meteor(Meteorite_t *meteor)
{
    int i, temp;
    temp = meteor->sx + meteor->height;
    for(i = 0; i < meteor->height; i++)
    {
        meteor->sx = temp - i;
        _clear_meteor(meteor);
    }
}
/* Ham nay co chuc nang tao ra 1 thien thach voi kich thuoc va vi tri ngau nhien tren top screen */
Meteorite_t *create_meteorite(void)
{
    
    Meteorite_t *meteor = (Meteorite_t *)malloc(sizeof(Meteorite_t));
    if(NULL == meteor)
    {
        printf("out of memory to create meteor");
        return NULL;
    }
    meteor->sx = 0;
    srand(time(0)); 
    meteor->scale = rand() % 3;
    switch (meteor->scale)
    {
    case SMALL:
        meteor->sy = rand() % (win_col - 2);    /* 2 is length of small string meteor */
        meteor->height = 1;
        meteor->health = 10;
        meteor->length = 2;
        break;
    case MEDIUM:
        meteor->sy = rand() % (win_col - 5);    /* 5 is length of medium string meteor */
        meteor->height = 2;
        meteor->health = 20;
        meteor->length = 5;
        break;
    case LARGE:
        meteor->sy = rand() % (win_col - 10);   /* 10 is length of large string meteor */
        meteor->height = 4;
        meteor->health = 30;
        meteor->length = 10;
        break;
    }
    meteor->speed = 1;
    return meteor;
}

void *handle_meteorite(void *arg)
{
    int rt_check = 0;
    Meteorite_t *meteorx = create_meteorite();
    while(PLAYING_STATUS == game_status)
    {
        if(NULL == meteorx)
        {
            break;
        }
        draw_meteorite(meteorx);
        _clear_meteor(meteorx);
        usleep(SPEED_METEORITE);
        if (meteorx->sx > (win_line + 4))
        {
            break;
        }
        rt_check = check_coordinate_meteor(meteorx->sx, meteorx->sy, meteorx->length);
        if(BULLET == rt_check)
        {
            meteorx->health = meteorx->health - 10;
            if(0 == meteorx->health)
            {
                add_score(meteorx->scale);
                _destroy_meteor(meteorx);
                break;
            }        
        }
        else if(SHIP == rt_check)
        {
            add_score(meteorx->scale);
            lose_one_life();
            _destroy_meteor(meteorx);
            /* The ship loses 1 life*/
            break;
        }
    }
    free(meteorx);
    pthread_exit(NULL);
}