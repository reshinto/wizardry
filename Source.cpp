// #include<conio.h>  // required if using windows

#include<stdio.h>  // required for prinf()
#include<stdlib.h>  // required for system()
#include<string.h>
#include"header/aa.h"
#include"header/aad.h"
#include<iostream>

// required for getch() to work in mac os
#include <unistd.h>
#include <termios.h>
char _getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

#define AA_WIDTH 28  // ascii width
#define AA_HEIGHT 28  // ascii height
#define LOCATION_MAX 15  // max number of locations in one direction

// Map size
#define FIELD_WIDTH 20
#define FIELD_HEIGHT 20

enum {
    DIRECTION_NORTH,
    DIRECTION_WEST,
    DIRECTION_SOUTH,
    DIRECTION_EAST,
    DIRECTION_MAX
};

enum {
    WALL_TYPE_NONE,
    WALL_TYPE_WALL,
    WALL_TYPE_DOOR,
    WALL_TYPE_MAX
};

// Map location information
// 2 refers to 2D
// up & left is negative, down & right is positive
int locations[DIRECTION_MAX][LOCATION_MAX][2] = {
    // DIRECTION_NORTH
    {
        {-1,-4}, {1,-4}, {0,-4},
        {-1,-3}, {1,-3}, {0,-3},
        {-1,-2}, {1,-2}, {0,-2},
        {-1,-1}, {1,-1}, {0,-1},
        {-1,-0}, {1,-0}, {0,-0}
    },
    // DIRECTION_WEST
    {
        {-4,1}, {-4,-1}, {-4,0},
        {-3,1}, {-3,-1}, {-3,0},
        {-2,1}, {-2,-1}, {-2,0},
        {-1,1}, {-1,-1}, {-1,0},
        {-0,1}, {-0,-1}, {-0,0}
    },
    // DIRECTION_SOUTH
    {
        {1,4}, {-1,4}, {0,4},
        {1,3}, {-1,3}, {0,3},
        {1,2}, {-1,2}, {0,2},
        {1,1}, {-1,1}, {0,1},
        {1,0}, {-1,0}, {0,0}
    },
    // DIRECTION_EAST
    {
        {4,-1}, {4,1}, {4,0},
        {3,-1}, {3,1}, {3,0},
        {2,-1}, {2,1}, {2,0},
        {1,-1}, {1,1}, {1,0},
        {0,-1}, {0,1}, {0,0}
    }
};

// wall data for the map of size 20x20, 0 = no wall, 1 = have wall
// 2 = door
// {northWall, westWall, southWall, eastWall}
int walls[FIELD_HEIGHT][FIELD_WIDTH][DIRECTION_MAX] = {
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 0},
        {1, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 0},
        {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {1, 1, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0},
        {0, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {0, 0, 0, 0},
        {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0},
        {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 1}, {0, 1, 0, 0},
        {0, 0, 1, 0}, {2, 0, 1, 0}, {1, 0, 1, 0}, {2, 0, 1, 0}, {1, 0, 1, 0},
        {2, 0, 1, 0}, {1, 0, 1, 0}, {2, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 2}
    },
    {
        {1, 1, 0, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 0, 0}, {1, 0, 1, 0},
        {1, 0, 1, 0}, {1, 0, 0, 0}, {1, 0, 1, 0}, {1, 0, 1, 2}, {0, 2, 1, 1},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 1, 2, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 0, 0},
        {1, 1, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 1}, {0, 0, 0, 0},
        {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 2}, {2, 2, 0, 0},
        {2, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 2, 1}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 2, 0, 0},
        {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {1, 1, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 1},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 2, 2, 0},
        {0, 0, 2, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1},
        {0, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 1}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 2, 0}, {0, 0, 1, 1}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
    {
        {0, 1, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0}, {1, 0, 1, 0},
        {1, 0, 1, 0}, {1, 0, 1, 0}, {2, 0, 1, 0}, {1, 0, 1, 1}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
        {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}
    },
};

// ascii art for walls
char *aa[LOCATION_MAX][DIRECTION_MAX] = {
    {NULL,NULL,aa0_2,NULL}, {NULL,NULL,aa1_2,NULL}, {NULL,NULL,aa2_2,NULL},
    {aa3_0,NULL,aa3_2,aa3_3}, {aa4_0,aa4_1,aa4_2,NULL}, {aa5_0,aa5_1,aa5_2,aa5_3},
    {aa6_0,NULL,aa6_2,aa6_3}, {aa7_0,aa7_1,aa7_2,NULL}, {aa8_0,aa8_1,aa8_2,aa8_3},
    {aa9_0,NULL,aa9_2,aa9_3}, {aa10_0,aa10_1,aa10_2,NULL}, {aa11_0,aa11_1,aa11_2,aa11_3},
    {aa12_0,NULL,NULL,aa12_3}, {aa13_0,aa13_1,NULL,NULL}, {aa14_0,aa14_1,NULL,aa14_3}
};

// ascii art for doors
char *aad[LOCATION_MAX][DIRECTION_MAX] = {
    {NULL,NULL,aa0_2d,NULL}, {NULL,NULL,aa1_2d,NULL}, {NULL,NULL,aa2_2d,NULL},
    {aa3_0d,NULL,aa3_2d,aa3_3d}, {aa4_0d,aa4_1d,aa4_2d,NULL}, {aa5_0d,aa5_1d,aa5_2d,aa5_3d},
    {aa6_0d,NULL,aa6_2d,aa6_3d}, {aa7_0d,aa7_1d,aa7_2d,NULL}, {aa8_0d,aa8_1d,aa8_2d,aa8_3d},
    {aa9_0d,NULL,aa9_2d,aa9_3d}, {aa10_0d,aa10_1d,aa10_2d,NULL}, {aa11_0d,aa11_1d,aa11_2d,aa11_3d},
    {aa12_0d,NULL,NULL,aa12_3d}, {aa13_0d,aa13_1d,NULL,NULL}, {aa14_0d,aa14_1d,NULL,aa14_3d}
};

int playerX = 0,
    playerY = 19;
int playerDirection;

// boss ascii art
char murphy[] = "\
（｜｜｜）\n\
　◎ 皿◎\n\
　／｜＞\n\
｜　◯\n\
◯　＼＼\n\
　　／／\n\
";

char murphySmile[] = "\
（｜｜｜）\n\
ｄへ皿へ\n\
＼／｜＞\n\
　　◯\n\
　　＼＼\n\
　　／／\n\
";

void ending() {
    system("clear");
    printf("%s", murphy);
    printf("やあ冒険者君。");
    _getch();

    system("clear");
    printf("%s", murphy);
    printf("あまりボクをいじめない方がいい。");
    _getch();

    system("clear");
    printf("%s", murphy);
    printf("なぜならボクは・・・");
    _getch();

    system("clear");
    printf("%s", murphy);
    printf("ノーリスクでレベル上げが出来る、\n");
    _getch();
    printf("このゲームのバランスブレイカーだからだ。");
    _getch();

    system("clear");
    printf("%s", murphy);
    printf("まあ君の自由だけど\n");
    _getch();
    printf("お手柔らかに頼むよ。");

    system("clear");
    printf("%s", murphySmile);
    printf("それでは良いWizライフを！\n");
    _getch();

    system("clear");
    printf("THE END\a");
    _getch();

    exit(0);
}

int main(){
    while(1){
        char buffer[AA_WIDTH * AA_HEIGHT];
        // memcpy(buffer, aaAll, sizeof buffer);
        memset(buffer, ' ', sizeof buffer);
        for(int i=0; i<LOCATION_MAX; i++){
            int x = playerX + locations[playerDirection][i][0];
            int y = playerY + locations[playerDirection][i][1];
            x = (FIELD_WIDTH + x) % FIELD_WIDTH;  // allow loops even with -ve sign
            y = (FIELD_HEIGHT + y) % FIELD_HEIGHT;  // allow loops even with -ve sign
            for(int j=0; j<DIRECTION_MAX; j++){
                int direction = (playerDirection + j) % DIRECTION_MAX;
                char *src = NULL;
                switch(walls[y][x][direction]){
                    case WALL_TYPE_WALL:src = aa[i][j]; break;
                    case WALL_TYPE_DOOR:src = aad[i][j]; break;
                }
                if(src != NULL){
                    for(int k=0; k<AA_WIDTH*AA_HEIGHT; k++){
                        if(src[k] != ' '){
                            buffer[k] = src[k];
                        }
                    }
                }
            }
        }

        // clear screen
        system("clear");
        
        // Convert full-width characters to half-width characters
        for(int y=0; y<AA_HEIGHT; y++){
            for(int x=0; x<AA_WIDTH; x++){
                switch(buffer[y*AA_WIDTH + x]){
                    case ' ':printf("　"); break;
                    case '.':printf("・"); break;
                    case '/':printf("／"); break;
                    case '?':printf("＼"); break;
                    case '|':printf("｜"); break;
                    case '_':printf("＿"); break;
                    default:printf("？"); break;
                }
            }
            printf("\n");
        }
        // use for debugging
        // printf("x:%d, y:%d, d%d", playerX, playerY, playerDirection);
        switch (_getch()){
            case 'w':
                {
                    int v[][2] = {
                        // DIRECTION_NORTH
                        {0, -1},
                        // DIRECTION_WEST
                        {-1, 0},
                        // DIRECTION_SOUTH
                        {0, 1},
                        // DIRECTION_EAST
                        {1, 0}
                    };
                    // Need to add FIELD_WIDTH and field_height, else will be -ve
                    playerX = (FIELD_WIDTH + playerX + v[playerDirection][0]) % FIELD_WIDTH;
                    playerY = (FIELD_HEIGHT + playerY + v[playerDirection][1]) % FIELD_HEIGHT;
                    if((playerX == 5) && (playerY == 10)){
                        playerX = 15;
                        playerY = 15;
                    } else if ((playerX == 13) && (playerY == 14)){
                        ending();
                    }
                }
                break;
            case 's':playerDirection = (playerDirection + 2) % DIRECTION_MAX; break;
            case 'a':playerDirection = ++playerDirection % DIRECTION_MAX; break;
            case 'd':playerDirection = (DIRECTION_MAX + --playerDirection) % DIRECTION_MAX; break;
        }
    }
}
