#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

key_t key = 5124;
int *marketstock;
pthread_t tid;
int stat = 3;    

void *updater(void* arg){
    while(stat){
        redrawwin(stdscr);
        mvprintw(0,0,"Food Market");
        mvprintw(2,0, "Market Food Stock\t: %d", *marketstock);
        mvprintw(16,0,"Selection:");
        mvprintw(17,0,"1. Restock");
        mvprintw(18,0,"2. Exit");
        move(19,0);
        refresh();
        stat = getch();
        switch (stat)
        {
            case '1':
                mvprintw(22,30,"1 food stocked");
                (*marketstock)++;
                napms(600);    
                break;
            case '2': stat = 0;
            default:
                break;
        }
        refresh();
        napms(100);    
    }
}

int main(){
    int shmid = shmget(key, sizeof(int), 0666);
    marketstock = shmat(shmid, NULL, 0);
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    pthread_create(&tid, NULL, updater, NULL);

    pthread_join(tid, NULL);
    shmdt(marketstock);
    // shmctl(shmid, IPC_RMID, NULL);
    endwin();
    return 0;
}