#include "meteorite.h"

//extern pthread_mutex_t meteo_mutex;
pthread_mutex_t meteo_mutex;
static int win_x, win_y;
static void _clear_meteor(Meteorite_t *meteor);
void draw_meteorite(Meteorite_t *meteor)
{
    pthread_mutex_lock(&meteo_mutex);
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
    meteor->sx +=1;
    pthread_mutex_unlock(&meteo_mutex);
}
static void _clear_meteor(Meteorite_t *meteor)
{
    pthread_mutex_lock(&meteo_mutex);
    switch (meteor->scale)
    {
    case SMALL:
        gotoxy(meteor->sx  - 1,meteor->sy);
        printf("  ");
        break;
    case MEDIUM:
        
        gotoxy(meteor->sx  - 2,meteor->sy);
        printf("     ");

        break;
    case LARGE:

        gotoxy(meteor->sx  - 4,meteor->sy);
        printf("          ");
        
        break;
    }
    pthread_mutex_unlock(&meteo_mutex);
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
        meteor->sy = rand() % (win_y - 2);    /* 2 is length of small string meteor */
        meteor->meteo_interval = 1;
        break;
    case MEDIUM:
        meteor->sy = rand() % (win_y - 5);    /* 5 is length of medium string meteor */
        meteor->meteo_interval = 2;
        break;
    case LARGE:
        meteor->sy = rand() % (win_y - 10);   /* 10 is length of large string meteor */
        meteor->meteo_interval = 4;
        break;
    }
    meteor->speed = 1;
    return meteor;
}

void *handle_meteorite(void *arg)
{
    int count = 0;
    Meteorite_t *meteorx = create_meteorite();
    get_window_size(&win_x,&win_y);
    while(1)
    {
        draw_meteorite(meteorx);
        count++;
        if(count > (meteorx->meteo_interval))
        {
            _clear_meteor(meteorx);
        }
        usleep(50000);
        if(meteorx->sx > win_x + 4)
        {
            break;
        }
    }
    free(meteorx);
    pthread_exit(NULL);
}