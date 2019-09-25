#include <stdio.h>
#include <pthread.h>
#include "ship.h"
#include "keyboard.h"
#include "utilities.h"
#include "bullet.h"

int game_status = PLAYING_STATUS;
Spaceship_t *my_ship = NULL;
extern keyQueue_t *keyhead;
pthread_mutex_t kqmutex;

void *generate_bullet(void *arg)
{
    pthread_t bullet[1000];
    int i = 0;
    while(PLAYING_STATUS == game_status)
    {
        pthread_create(&bullet[i],NULL, handle_bullet, (void *)my_ship);
        //thread_join(bullet[i], NULL);
        i = i + 1;
        usleep(50000);
        if(1000 == i)
        {
            i = 0;
        }
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
    pthread_t thread[3];
    printf("\e[?25l\033[H\033[J");   /*clear screen and hide cursor */
    my_ship = init_spaceship();
    draw_spaceship(my_ship);
    pthread_create(&thread[0], NULL, read_keyboard,NULL );
    pthread_create(&thread[1], NULL, control,NULL );
    pthread_create(&thread[2], NULL, generate_bullet,NULL );
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL); 
    pthread_join(thread[2], NULL); 
    return 0;
}
int get_coordinate_ship(int *x, int *y)
{
    if(NULL != my_ship)
    {
        *x = my_ship->sx;
        *y = my_ship->sy;
        return 0;
    }
    return -1;
}