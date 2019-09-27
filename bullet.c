#include "bullet.h"

extern pthread_mutex_t draw_mutex;
extern int win_col;
extern int map_arr[MAX_ROW][MAX_COLUMN];
extern pthread_mutex_t access_map_arr_mutex;
Bullet_t * create_bullet(int x, int y)  /* x and y are cordinates of the ship */
{
    Bullet_t *bullet =  (Bullet_t *)malloc(sizeof(Bullet_t));
    bullet->sx = x - 1; /* center of the ship*/
    bullet->sy = y + 4;
    bullet->gone = false;

    return bullet;
}
static void _update_position_bullet(Bullet_t *bullet, int value)
{
    pthread_mutex_lock(&access_map_arr_mutex);
    map_arr[bullet->sx][bullet->sy] = value;
    pthread_mutex_unlock(&access_map_arr_mutex);    
}
static void _draw_bullet(Bullet_t *bullet)
{
    pthread_mutex_lock(&draw_mutex);
    gotoxy(bullet->sx, bullet->sy);
    printf(BULLET_CHAR);
    pthread_mutex_unlock(&draw_mutex);
    _update_position_bullet(bullet, BULLET);
}
static void _clean_old_bullet(Bullet_t *bullet)
{
    pthread_mutex_lock(&draw_mutex);
    gotoxy(bullet->sx, bullet->sy);
    printf(" ");
    pthread_mutex_unlock(&draw_mutex);
    _update_position_bullet(bullet, EMPTY);
}
void *handle_bullet(void *arg)
{
    Spaceship_t *uship = (Spaceship_t *)arg;
    Bullet_t *mbullet = create_bullet(uship->sx, uship->sy);
    while(1)
    {    
        if(METEOR == check_coordinate_bullet(mbullet->sx,mbullet->sy)) 
        {
            break;
        }

        if(mbullet->sx >= 1)
        {
            _draw_bullet(mbullet);
            usleep(BULLET_SPEED);
            /* clean old position of the bullet - replace '*' with ' ' */
            _clean_old_bullet(mbullet);
            mbullet->sx -= 1;

        }
        else 
        {
            mbullet->gone = true;
            break;
        }
    } 
    free(mbullet);
    pthread_exit(NULL);
    //return NULL;
}