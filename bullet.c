#include "bullet.h"

extern pthread_mutex_t draw_mutex;
Bullet_t * create_bullet(int x, int y)  /* x and y are cordinates of the ship */
{
    Bullet_t *bullet =  (Bullet_t *)malloc(sizeof(Bullet_t));
    bullet->sx = x - 1; /* center of the ship*/
    bullet->sy = y + 4;
    bullet->gone = false;

    return bullet;
}

void draw_bullet(Bullet_t *bullet)
{
    // gotoxy(0,0);
    // printf("x= %d,y= %d                            ",bullet->sx,bullet->sy);
    pthread_mutex_lock(&draw_mutex);
    gotoxy(bullet->sx, bullet->sy);
    printf("%c", BULLET_CHAR);
    pthread_mutex_unlock(&draw_mutex);
    
}
void clean_old_bullet(Bullet_t *bullet)
{
    pthread_mutex_lock(&draw_mutex);
    gotoxy(bullet->sx + 1, bullet->sy);
    printf(" ");
    pthread_mutex_unlock(&draw_mutex);
}
void *handle_bullet(void *arg)
{
    Spaceship_t *ship = (Spaceship_t *)arg;
    Bullet_t *mbullet = create_bullet(ship->sx, ship->sy);
    while(1)
    {    
        if(mbullet->sx >= 1)
        {
            draw_bullet(mbullet);
            usleep(20000);
            mbullet->sx -= 1;
            /* clean old position of the bullet - replace '*' with ' ' */
            clean_old_bullet(mbullet);

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