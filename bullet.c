#include "bullet.h"

extern pthread_mutex_t draw_mutex;
extern int win_col;
extern int map_arr[MAX_ROW][MAX_COLUMN];
extern pthread_mutex_t access_map_arr_mutex;
extern int game_status;
Bullet_t * create_bullet(int x, int y)  /* x and y are cordinates of the ship */
{
    Bullet_t *bullet =  (Bullet_t *)malloc(sizeof(Bullet_t));
    if(NULL == bullet)
    {
        printf("Out of memory to create bullet");
        return NULL;
    }
    bullet->sx = x - 1; /* center of the ship*/
    bullet->sy = y + 4;

    return bullet;
}

static void _draw_bullet(Bullet_t *bullet)
{
    pthread_mutex_lock(&draw_mutex);
    gotoxy(bullet->sx, bullet->sy);
    if (EMPTY == map_arr[bullet->sx][bullet->sy])
    {
        printf(BULLET_CHAR);
        map_arr[bullet->sx][bullet->sy] = BULLET;
    }
    pthread_mutex_unlock(&draw_mutex);
}
static void _clean_old_bullet(Bullet_t *bullet)
{
    pthread_mutex_lock(&draw_mutex);
    gotoxy(bullet->sx, bullet->sy);
    pthread_mutex_lock(&access_map_arr_mutex);
    if(BULLET == map_arr[bullet->sx][bullet->sy])
    {
        printf(" ");
        map_arr[bullet->sx][bullet->sy] = EMPTY;
    }
    pthread_mutex_unlock(&access_map_arr_mutex);   
    //printf(" ");
    pthread_mutex_unlock(&draw_mutex);
}
void *handle_bullet(void *arg)
{
    int rt_check;
    Spaceship_t *uship = (Spaceship_t *)arg;
    Bullet_t *mbullet = create_bullet(uship->sx, uship->sy);
    while(PLAYING_STATUS == game_status)
    {    
        rt_check = check_coordinate_bullet(mbullet->sx,mbullet->sy);
        if(NULL == mbullet)
        {
            break;
        }
        if(METEOR == rt_check) 
        {
            _clean_old_bullet(mbullet);
            break;
        }        
        else if (EMPTY == rt_check)
        {
            if (mbullet->sx >= 1)
            {
                _draw_bullet(mbullet);
                usleep(BULLET_SPEED);
                /* clean old position of the bullet - replace '*' with ' ' */
                _clean_old_bullet(mbullet);
                mbullet->sx -= 1;
               
            }
            else
            {
                break;
            }
        }
        //usleep(10);
    } 
    free(mbullet);
    pthread_exit(NULL);
}