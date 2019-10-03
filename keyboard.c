#include "keyboard.h"
#include "ship.h"

keyQueue_t *keyhead = NULL;
pthread_mutex_t kqmutex;
static struct termios origtc, newtc;

extern int game_status;

void set_default_terminal(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &origtc);                 /* settings back to normal */
    printf("\e[1;1H\e[2J");
}
void *read_keyboard(void *arg)
{
    tcgetattr(STDIN_FILENO, &origtc);
    newtc = origtc;
    newtc.c_lflag &= ~ICANON;                                 /* put in '1 key mode' */
    newtc.c_lflag &= ~ECHO;                                   /* turn off echo */
    while(PLAYING_STATUS == game_status)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &newtc);             /* echo off 1-key read mode */
        char key = getchar();                                /* get single key immed. */ 
        /* printf("Key '%c' pressed...\n", key); */
        if (NULL != strchr(GROUP_KEY, key))
        {
            pthread_mutex_lock(&kqmutex);
            keyQueue_t *kq = calloc(sizeof (struct keyQueue), 1); /* allocate pkt */
            kq->key = key;    
            kq->next = NULL;                   
            keyQueue_t *kptr = keyhead;                      /* get pointer to queue head */
            if(NULL == keyhead)
            {
                keyhead = kq;
            }
            else   /* insert from last */
            {
                while(NULL != kptr->next)                        /* find first empty slot */
                kptr = kptr->next;
                kptr->next = kq;                                 /* enqueue key packet to thread */
            }
            pthread_mutex_unlock(&kqmutex);
        } 
        usleep(10);                
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &origtc);                 /* settings back to normal */
    pthread_exit(NULL);
}
