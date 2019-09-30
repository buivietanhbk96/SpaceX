#include <stdio.h>
#include <pthread.h>
#include "ship.h"
#include "keyboard.h"
#include "utilities.h"
#include "bullet.h"
#include "meteorite.h"


#define MAX_BULLET 5000             /* number of thread use for create bullet (each bullet is one thread) */
#define MAX_METEOR 5000             /* number of thread use for create meteorite (each meteorite is one thread) */
#define SPEED_GEN_BULLET 100000     /* period to create bullet  (us)*/
#define SPEED_GEN_METEORITE 5       /* period to create bullet  (s)*/

int map_arr[MAX_ROW][MAX_COLUMN];
int game_status = PLAYING_STATUS;
Spaceship_t *my_ship = NULL;
extern keyQueue_t *keyhead;
pthread_mutex_t kqmutex;
pthread_mutex_t access_map_arr_mutex;
void *display_score(void *arg)
{
    while(GAMEOVER_STATUS != game_status)
    {
        usleep(10);
        draw_score();
    }
    pthread_exit(NULL);
}
void *generate_bullet(void *arg)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 1);
    pthread_t bullet[MAX_BULLET];
    int i = 0;
    while(PLAYING_STATUS == game_status)
    {
        if (MAX_BULLET == i)
        {
            i = 0;
        }
        usleep(SPEED_GEN_BULLET);
        pthread_create(&bullet[i],&attr, handle_bullet, (void *)my_ship);
        /* pthread_join(bullet[i], NULL);  don't use pthread_join at here*/
        /* pthread_detach(bullet[i]); */
        i = i + 1;
    }
    pthread_attr_destroy(&attr);
    printf("end bullet\n");
    pthread_exit(NULL);
}

void *generate_meteorite(void *arg)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 1);
    pthread_t meteorites[MAX_METEOR];
    int i = 0;
    while(PLAYING_STATUS == game_status)
    {
        if (MAX_METEOR == i)
        {
            i = 0;
        }
        pthread_create(&meteorites[i],&attr, handle_meteorite, NULL);
        /* pthread_join(meteorites[i], NULL);  don't use pthread_join at here*/
        //pthread_detach(meteorites[i]);
        i = i + 1;
        sleep(SPEED_GEN_METEORITE);
    }
    pthread_attr_destroy(&attr);
    printf("end meteo\n");
    pthread_exit(NULL);
}
void *control(void *arg)
{
    char key;
    while(PLAYING_STATUS == game_status)
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
    printf("end control\n");
    pthread_exit(NULL);
}
static void _create_map()
{
    memset(map_arr, 0, sizeof(int) * MAX_ROW * MAX_COLUMN);
}
void print_map_arr()
{
    int i, j;
    for (i = 0; i < 60; ++i)
    {
        for (j = 0; j < 210; ++j)
            printf("%d ", map_arr[i][j]);
        printf("\n");
    }
}
int main(void)
{
    pthread_t thread[5];
    printf("\e[?25l\033[H\033[J");   /*clear screen and hide cursor */
    _create_map();
    my_ship = init_spaceship();
    draw_spaceship(my_ship);
    pthread_create(&thread[0], NULL, read_keyboard,NULL );
    pthread_create(&thread[1], NULL, control,NULL );
    pthread_create(&thread[2], NULL, generate_meteorite ,NULL );
    pthread_create(&thread[3], NULL, generate_bullet ,NULL );
    pthread_create(&thread[4], NULL, display_score ,NULL );
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL); 
    pthread_join(thread[2], NULL); 
    pthread_join(thread[3], NULL); 
    pthread_join(thread[4], NULL); 
    printf("GAME OVER\n");
    return 0;
}
