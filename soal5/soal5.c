// #include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

char name[50];
float hunger = 200;
float hygiene = 100;
float health = 300;
int foodnum = 0;
int lifetime = 0;
int wons = 0;
WINDOW *win, *batt, *market;
bool game = true;
int messagecounter = 0;
float bathstat = 0;
char available[50]  = "Bath is available";
key_t key = 5124;
int *marketstock;

void drawmain();
void updatedata();
void shop();
void gameover();
int battle();

int main(int argc, char *argv[]){
    if(argc == 2)sprintf(name, "%s", argv[1]);
    else{
    //     printf("Please input your monsters name: ");
    //     scanf("%s", name);
    }
    
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    marketstock = shmat(shmid, NULL, 0);

    *marketstock = 5;

    win = initscr();
    batt = newwin(24,70,0,0);
    market = newwin(24,70,0,0);
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    scrollok(batt, TRUE);
    nodelay(batt, TRUE);
    drawmain();
    while(game){
        updatedata();
        char input = getch();
        switch (input)
        {
            case '1':
                if(foodnum){
                    foodnum--;
                    hunger += 15;
                }else {
                    messagecounter = 30;
                    mvprintw(23,30, "\t\t\t\t\t");
                    mvprintw(23, 30, "insufficient food..");
                    redrawwin(stdscr);
                }
                if(hunger>200)hunger=200;
                break;
            
            case '2':
                if(bathstat > 0){
                    messagecounter = 30;
                    mvprintw(23,30, "\t\t\t\t\t");
                    mvprintw(23, 30, "Bath is not ready yet..");
                    redrawwin(stdscr);
                }else{
                    bathstat = 20;
                    hygiene += 30;
                    if(hygiene > 100) hygiene = 100;
                    messagecounter = 30;
                    mvprintw(23,30, "\t\t\t\t\t");
                    mvprintw(23, 30, "%s is feel refreshed!!",name);
                    redrawwin(stdscr);
                }
                break;

            case '3':
                messagecounter = 30;
                if(battle() == 0)
                mvprintw(23, 30, "You have fled from battlefield..");
                else {
                    mvprintw(23,30, "\t\t\t\t\t");
                    mvprintw(23, 30, "You Won!!!");
                    wons++;
                }
                redrawwin(stdscr);
                break;
        
            case '4':
                shop();
                break;
        
            case '5':
                game = false;
                break;
        
            default:
                break;
        }
        lifetime ++;
        if(lifetime%3==0)hygiene -= 0.1;
        hunger -= 0.05;
        if(health < 300)health += 0.05;
        if(messagecounter-- == 0){
            // system("clear");
            mvprintw(23, 30, "\t\t\t\t\t");
            redrawwin(stdscr);
        }
        if(bathstat > 0) bathstat -= 0.1;
        napms(100);
        if(health<0 || hunger<0 || hygiene<0)game = false;
    }
    gameover();
    shmdt(marketstock);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

int battle(){
    int stat = 1;
    int enemyhealth = 100;
    system("clear");
    while(stat){
        redrawwin(batt);
        mvwprintw(batt, 0,20, "%s VS zombie",name);
        mvwprintw(batt, 1,20, "%d      %d",(int)health,enemyhealth);
        mvwprintw(batt,16,0,"Selection:");
        mvwprintw(batt,17,0,"1. Attack");
        mvwprintw(batt,18,0,"2. Flee");
        move(19,0);
        wrefresh(batt);
        stat = getch() - '0';
        switch (stat)
        {
            case 1:
                mvwprintw(batt,22,30,"%s attack!!", name);
                wrefresh(batt);
                napms(300);
                if(rand()%10 > 1){
                    mvwprintw(batt,23,30,"zombie took 20 damage");
                    enemyhealth -= 20;
                    mvwprintw(batt, 1,20,"\t\t\t\t\t\t");
                    mvwprintw(batt, 1,20, "%d      %d",(int)health,enemyhealth);
                } else mvwprintw(batt,23,30,"missed!!");
                move(19,0);
                redrawwin(batt);
                wrefresh(batt);
                napms(1000);
                if(enemyhealth <= 0)return 1;
                mvwprintw(batt,22,30,"zombie attack!!");
                mvwprintw(batt,23,30,"\t\t\t\t\t");
                wrefresh(batt);
                napms(300);
                if(rand()%10 > 4){
                    mvwprintw(batt,23,30,"%s took 20 damage", name);
                    health -= 20;
                    mvwprintw(batt, 1,20,"\t\t\t\t\t\t");
                    mvwprintw(batt, 1,20, "%d      %d",(int)health,enemyhealth);
                } else mvwprintw(batt,23,30,"missed!!");
                redrawwin(batt);
                wrefresh(batt);
                move(19,0);
                napms(1000);
                mvwprintw(batt,22,30,"\t\t\t\t\t");
                mvwprintw(batt,23,30,"\t\t\t\t\t");
                break;

            case 2:
                return 0;
            default:
                break;
        }
        if(enemyhealth <= 0)return 1;
        if(health <= 0)gameover();
        napms(100);
    }
    return rand()%2;
}

void shop(){
    int stat = 3;
    while(stat){
        redrawwin(market);
        mvwprintw(market,0,0,"Food Market");
        mvwprintw(market,2,0, "Market stock\t: %d", *marketstock);
        mvwprintw(market,3,0, "Your Stock\t: %d", foodnum);
        mvwprintw(market,16,0,"Selection:");
        mvwprintw(market,17,0,"1. Buy");
        mvwprintw(market,18,0,"2. Back");
        move(19,0);
        wrefresh(market);
        stat = getch();
        switch (stat)
        {
            case '1':
                if(*marketstock > 0){
                    (*marketstock)--;
                    foodnum++;
                    mvwprintw(market,22,30,"1 food purcashed");
                }else   mvwprintw(market,22,30,"out of stock");
                mvwprintw(market,2,0, "Market stock\t: %d", *marketstock);
                mvwprintw(market,3,0, "Your Stock\t: %d", foodnum);
                wrefresh(market);
                napms(600);    
                break;
            case '2': return;
            default:
                break;
        }
        mvwprintw(market,22,30,"\t\t\t\t\t");
        wrefresh(market);
        napms(100);    
    }
}

void drawmain(){
    mvprintw(0,0,"Name\t: %s",name);
    mvprintw(1,0,"Health\t: %d", (int)health);
    mvprintw(2,0,"Hunger\t: %d", (int)hunger);
    mvprintw(3,0,"Hygiene\t: %d", (int)hygiene);
    mvprintw(0,30,"Lifetime\t: %d seconds", lifetime/10);
    mvprintw(1,30,"Battles won\t: %d", wons);
    mvprintw(3,30,"%s", available);
    mvprintw(2,30,"Food owned\t: %d", foodnum);

    mvprintw(7,10,"  _____");
    mvprintw(8,10," / . . \\");
    mvprintw(9,10,"|  \\_/  |");
    mvprintw(10,10,"\\       /");
    mvprintw(11,10," []---[]");

    mvprintw(16,0, "Selections:");
    mvprintw(17,3,"1. Eat");
    mvprintw(18,3,"2. Bath");
    mvprintw(19,3,"3. Battle");
    mvprintw(20,3,"4. Shop");
    mvprintw(21,3,"5. Exit");
}

void updatedata(){
    system("clear");
    redrawwin(stdscr);
    mvprintw(1,10,"\t");
    mvprintw(2,10,"\t");
    mvprintw(3,10,"\t");
    mvprintw(0,42,"\t");
    mvprintw(1,50,"\t");
    mvprintw(2,50,"\t");
    mvprintw(3,30,"\t\t\t\t");

    mvprintw(1,10,"%.2f", health);
    mvprintw(2,10,"%.2f", hunger);
    mvprintw(3,10,"%.2f", hygiene);
    mvprintw(0,42,"%d seconds", lifetime/10);
    mvprintw(1,50,"%d", wons);
    mvprintw(2,50,"%d", foodnum);

    char bathmsg[50];
    if(bathstat<=0)sprintf(bathmsg,"[%s]",available);
    else sprintf(bathmsg, "Bath will be ready in %d s",(int)bathstat);
    mvprintw(3,30,"%s", bathmsg);

    refresh();
    move(23,75);
}

void gameover(){
    system("clear");
    mvwprintw(batt,0,0,"GAME");
    wrefresh(batt);
    napms(700);
    wprintw(batt," OVER");
    wrefresh(batt);
    napms(700);
    wprintw(batt,".");
    wrefresh(batt);
    napms(1000);
    // delwin(batt);
    // delwin(stdscr);
    endwin();
    // system("clear");
}