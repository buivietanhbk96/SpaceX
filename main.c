#include <stdio.h>
#include <pthread.h>
#include "ship.h"
#include "keyboard.h"
#include "utilities.h"
#include "bullet.h"
#include "meteorite.h"


#define MAX_BULLET 1000             /* number of thread use for create bullet (each bullet is one thread) */
#define MAX_METEOR 1000             /* number of thread use for create meteorite (each meteorite is one thread) */
#define SPEED_GEN_BULLET 80000      /* period to create bullet  (us)*/
#define SPEED_GEN_METEORITE 5       /* period to create bullet  (s)*/
int game_status = PLAYING_STATUS;
Spaceship_t *my_ship = NULL;
extern keyQueue_t *keyhead;
pthread_mutex_t kqmutex;

void *generate_bullet(void *arg)
{
    pthread_t bullet[MAX_BULLET];
    int i = 0;
    while(PLAYING_STATUS == game_status)
    {
        if (MAX_BULLET == i)
        {
            i = 0;
        }
        pthread_create(&bullet[i],NULL, handle_bullet, (void *)my_ship);
        /* pthread_join(bullet[i], NULL);  don't use pthread_join at here*/
        i = i + 1;
        usleep(SPEED_GEN_BULLET);

    }
    pthread_exit(NULL);
}

void *generate_meteorite(void *arg)
{
    pthread_t meteorites[MAX_METEOR];
    int i = 0;
    while(PLAYING_STATUS == game_status)
    {
        if (MAX_METEOR == i)
        {
            i = 0;
        }
        pthread_create(&meteorites[i],NULL, handle_meteorite, NULL);
        /* pthread_join(meteorites[i], NULL);  don't use pthread_join at here*/
        i = i + 1;
        sleep(SPEED_GEN_METEORITE);
    }
    pthread_exit(NULL);
}
void *control(void *arg)
{
    char key;
    while(1)
    {
        pthread_mutex_lock(&kqmutex);        /* lock other threads out while we tamper */
        key = '\0';                          /* initialize key to NULL*/
        if (NULL != keyhead) {               /* Anything queued up for us? */
            keyQueue_t *kq = keyhead; 
            key = kq->key;                   
            keyhead = kq->next;       
            free(kq);
        }  
        pthread_mutex_unlock(&kqmutex);   
        if (key != '\0') {                   
            switch(key)
            {
                case 'a': 
                {
                    /* move the ship left */
                    move_spaceship(my_ship, LEFT);
                    break;
                }
                case 'd':
                {
                    /* move the ship left */
                    move_spaceship(my_ship, RIGHT);
                    break;                    
                }
                case 's':
                {
                    /* move the ship down */
                    move_spaceship(my_ship, DOWN);
                    break;                    
                }
                case 'w':
                {
                    /* move the ship up */
                    move_spaceship(my_ship, UP);
                    break;                    
                }                          
            }
        }
    }
    pthread_exit(NULL);
}
int main(void)
{
    pthread_t thread[4];
    printf("\e[?25l\033[H\033[J");   /*clear screen and hide cursor */
    my_ship = init_spaceship();
    draw_spaceship(my_ship);
    pthread_create(&thread[0], NULL, read_keyboard,NULL );
    pthread_create(&thread[1], NULL, control,NULL );
    pthread_create(&thread[2], NULL, generate_meteorite ,NULL );
    pthread_create(&thread[3], NULL, generate_bullet ,NULL );
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL); 
    pthread_join(thread[2], NULL); 
    pthread_join(thread[3], NULL); 
    return 0;
}
