// Terminal size must be 92 x 46 : (UNIX)resize -s 46 92 (WIN(PowerShell)) mode con: cols=92 lines=46
// To compile : (UNIX) clang mysnakegame.c -o snakegame -lncurses  OR  (WIN) gcc mysnakegame.c -o snakegame.exe -lncurses
// To execute : (UNIX)./snakegame  OR  (WIN)./snakegame.exe
 

#include<curses.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<stdio.h>

#define BOX_WIDTH 40
#define BOX_HEIGHT 40
#define MAX_SEGMENTS 100

float k = 1.75;

typedef struct{
    int x;
    int y;
}vec;

typedef struct {
    vec segments[MAX_SEGMENTS];
    int length;
} snake;


int score = 0;

vec berry = {5 + (BOX_WIDTH/2),10 + (BOX_HEIGHT/2)};
vec snake_head = {6,11};
vec snake_dir = {1,0};

void spawn_berry(vec *berry){

    berry->x = 1 + rand()%(BOX_WIDTH-2);
    berry->y = 1 + rand()%(BOX_HEIGHT-2);

}

void draw_box(int height,int width){

    start_color();
    init_pair(1,COLOR_WHITE,COLOR_GREEN);

    attron(COLOR_PAIR(1));

    box(stdscr, '|', '-'); // draw a box around the window
    attroff(COLOR_PAIR(1));

    init_pair(2,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(2));
    mvprintw(0, BOX_WIDTH + 4, "[ Score: %d ]", score);
    attroff(COLOR_PAIR(2));
    refresh();
}

snake snake_body;

void init_snake(){
    snake_body.length = 1;
    snake_body.segments[0].x = snake_head.x;
    snake_body.segments[0].y = snake_head.y;
}

void move_snake(){
    for (int i = snake_body.length - 1; i > 0; i--) {
        snake_body.segments[i] = snake_body.segments[i - 1];
    }
    snake_body.segments[0] = snake_head;
}

int main(){

    bool gameover = false;
    //initialize screen
    
    start_color();
    bool revrse = false;
    int choice;
    printf("------------------------ Snake Mania -----------------------\n");
    printf("Select a Difficulty : \n");
    printf("1. Normal\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("4. I want to die right away\n");
    printf("5.Reverse Mode (Controls are Reversed)\n");
    printf("> ");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        k=1.5;
        break;
    case 2:
        k=2;
        break;
    case 3:
        k=2.5;
        break;
    case 4:
        k=5;
        break;
    case 5:
        revrse = true;
    default:
        k = 1.75;
        break;    
    }

    WINDOW *win = initscr(); // to initialize the window

    // take player input
    keypad(win,true); 
    nodelay(win,true); 
    curs_set(0);

    srand(time(NULL));

    while(!gameover){

        draw_box(BOX_HEIGHT,BOX_WIDTH);
        int pressed = wgetch(win);


        if(pressed == KEY_LEFT){
            if(revrse){
                if(snake_dir.x == -1) continue;
                snake_dir.x = 1;
                snake_dir.y = 0;
            }
            else{if(snake_dir.x == 1) continue;
            snake_dir.x = -1;
            snake_dir.y = 0;}
        }

        if(pressed == KEY_RIGHT){
            if(revrse){
                if(snake_dir.x == 1) continue;
            snake_dir.x = -1;
            snake_dir.y = 0;
            }
            else{
                if(snake_dir.x == -1) continue;
            snake_dir.x = 1;
            snake_dir.y = 0;
            }
        }

        if(pressed == KEY_UP){
            if(revrse){
                if(snake_dir.y == -1) continue;
            snake_dir.x = 0;
            snake_dir.y = 1;
            }
            else{
                if(snake_dir.y == 1) continue;
            snake_dir.x = 0;
            snake_dir.y = -1;
            }
        }

        if(pressed == KEY_DOWN){
           if(revrse){
             if(snake_dir.y == 1) continue;
            snake_dir.x = 0;
            snake_dir.y = -1;
           }
           else{
             if(snake_dir.y == -1) continue;
            snake_dir.x = 0;
            snake_dir.y = 1;
           }
        }

        if(pressed == '\e'){
            break;
        }

    snake_head.x += snake_dir.x;
    snake_head.y += snake_dir.y;

    if(snake_head.x == berry.x && snake_head.y == berry.y){
        score++;
        if (snake_body.length < MAX_SEGMENTS) {
                snake_body.length++;
                snake_body.segments[snake_body.length - 1] = snake_head;
            spawn_berry(&berry);
           
            }

                }

                    

    move_snake();

    if(snake_head.x == 46 || snake_head.x == 0 || snake_head.y == 0 || snake_head.y == 46){
        gameover = true;
    }

    erase();
    // start_color();
    init_pair(3,COLOR_RED,COLOR_YELLOW);
    init_pair(4,COLOR_WHITE,COLOR_MAGENTA);
    init_pair(5,COLOR_WHITE,COLOR_RED);

    if(score>=1){
    attron(COLOR_PAIR(4));
    mvaddch(berry.y,berry.x*2,'@');
    attroff(COLOR_PAIR(4));
    }
    else{
    attron(COLOR_PAIR(5));     
    mvaddch(berry.y,berry.x*2,'@');
    attroff(COLOR_PAIR(5));
    }
    
    attron(COLOR_PAIR(3));
    mvaddch(snake_head.y,snake_head.x*2,ACS_DIAMOND);

    for (int i = 1; i < snake_body.length; i++) {
            mvaddch(snake_body.segments[i].y, snake_body.segments[i].x * 2, ACS_BULLET);
        }

    for(int i=0;i<snake_body.length;i++){
        for(int j=0;j<snake_body.length;j++){
            if(snake_body.segments[i].x == snake_body.segments[j].x  && snake_body.segments[i].y == snake_body.segments[j].y && i!=j ){
                gameover = true;
                break;
            }

        }
    }    

    attroff(COLOR_PAIR(3));

    usleep(166000/k);

    }

   
    refresh();

    wgetch(win);

   

    endwin();
}

