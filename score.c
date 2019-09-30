#include "score.h"
static int score = 0;
pthread_mutex_t score_mutex;
extern pthread_mutex_t draw_mutex;
extern Spaceship_t *my_ship;
extern int win_line;
extern int game_status;
void add_score(int scale)
{
    pthread_mutex_lock(&score_mutex);
    switch (scale)
    {
    case SMALL:
        score += SMALL_SCORE;
        break;
    case MEDIUM:
        score += MEDIUM_SCORE;
        break;
    case LARGE:
        score += LARGE_SCORE;
        break;        
    }
    pthread_mutex_unlock(&score_mutex);
}
void lose_one_life()
{
    my_ship->lives -=1;
    if(0 == my_ship->lives)
    {
        game_status = GAMEOVER_STATUS;
    }
    pthread_mutex_lock(&draw_mutex);
    gotoxy(win_line, 20);
    printf("       ");
    pthread_mutex_unlock(&draw_mutex);
}

int get_score(void)
{
    return score;
}
void draw_score()
{
    int i;
    pthread_mutex_lock(&draw_mutex);
    gotoxy(win_line, 5);
    printf("SCORE = %d",score);
    for(i = 0; i < my_ship->lives; i++)
    {
        gotoxy(win_line, 20 + i + 1);
        printf(LIVE_CHAR);
    }
    pthread_mutex_unlock(&draw_mutex);
}