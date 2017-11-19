#include <stdio.h>

///////////////////////
/// constants start ///
///////////////////////

const enum Icons {
    EMPTY = '_',
    BLOCK = '#',
    CHEESE = '*',
    CHERRY = '^',
    pineapple = 'O',
} typedef Icons;

const double PACMAN_NORMAL = 1,
        PACMAN_FAST = 1.5,
        GHOST_AGGRESIVE = 1,
        GHOST_DEFENSIVE = 0.5;

const int CHESSE_VALUE = 1,
        CHERRY_VALUE = 20,
        GHOST_EAT_VALUE = 50;

/////////////////////
/// constants end ///
/////////////////////

////////////////////
/// models start ///
////////////////////

struct Score {
    int cheeseCount;
    int cherryCount;
    int pineappleCount;
    int totalScore;
} typedef Score;

const enum Direction {
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4,
} typedef Direction;

struct Point {
    int x;
    int y;
} typedef Point;

struct Pacman {
    Point startPoint;
    Point position;
    Direction direction;
    Score score;
    int hearts;
    double speed;
} typedef Pacman;

struct Ghost {
    Point startPoint;
    Point position;
    Direction direction;
    int isAggresive;
    int defensiveTimeLeft;
    double speed;
} typedef Ghost;

//////////////////
/// models end ///
//////////////////

/////////////////////////////
/// in game objects start ///
/////////////////////////////

int cycles;

int n, m;

Pacman pacman;

Ghost pinky;
Ghost blinky;
Ghost clyde;
Ghost inky;

///////////////////////////
/// in game objects end ///
///////////////////////////

/////////////////////
/// headers start ///
/////////////////////

void initialize();

void pacman_init();

void gameReport(char room[n][m]);

void run(char room[n][m]);

///////////////////
/// headers end ///
///////////////////

//////////////////
/// main start ///
//////////////////

int main() {
    initialize();
    scanf("%d %d\n", &n, &m);
    char room[n][m];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%c", &room[i][j]);
        }
        scanf("\n");
    }
    int min, sec;
    scanf("%d:%d", &min, &sec);
    cycles = min * 60 + sec;
    scanf("%d", &pacman.score.totalScore);
    /// init pacman ///
    scanf("%*s %d %d (%d,%d) (%d,%d)",
          &pacman.direction,
          &pacman.hearts,
          &pacman.startPoint.x,
          &pacman.startPoint.y,
          &pacman.position.x,
          &pacman.position.y
    );
    /// init blinky ///
    scanf("%*s %d %d",
          &blinky.direction,
          &blinky.isAggresive
    );
    if (blinky.isAggresive == 0)
        scanf("%d", &blinky.defensiveTimeLeft);
    else blinky.defensiveTimeLeft = 0;
    scanf(" (%d,%d) (%d,%d)",
          &blinky.startPoint.x,
          &blinky.startPoint.y,
          &blinky.position.x,
          &blinky.position.y
    );
    /// init pinky ///
    scanf("%*s %d %d",
          &pinky.direction,
          &pinky.isAggresive
    );
    if (pinky.isAggresive == 0)
        scanf("%d", &pinky.defensiveTimeLeft);
    else pinky.defensiveTimeLeft = 0;
    scanf(" (%d,%d) (%d,%d)",
          &pinky.startPoint.x,
          &pinky.startPoint.y,
          &pinky.position.x,
          &pinky.position.y
    );
    /// init clyde ///
    scanf("%*s %d %d",
          &clyde.direction,
          &clyde.isAggresive
    );
    if (clyde.isAggresive == 0)
        scanf("%d", &clyde.defensiveTimeLeft);
    else clyde.defensiveTimeLeft = 0;
    scanf(" (%d,%d) (%d,%d)",
          &clyde.startPoint.x,
          &clyde.startPoint.y,
          &clyde.position.x,
          &clyde.position.y
    );
    /// init inky ///
    scanf("%*s %d %d",
          &inky.direction,
          &inky.isAggresive
    );
    if (inky.isAggresive == 0)
        scanf("%d", &inky.defensiveTimeLeft);
    else inky.defensiveTimeLeft = 0;
    scanf(" (%d,%d) (%d,%d)",
          &inky.startPoint.x,
          &inky.startPoint.y,
          &inky.position.x,
          &inky.position.y
    );
    run(room);
    gameReport(room);
    return 0;
}

void run(char room[n][m]) {

}

////////////////
/// main end ///
////////////////

/////////////////////
/// methods start ///
/////////////////////

void initialize() {
    cycles = 0;
    pacman_init();
}

void pacman_init() {
    pacman.score.cheeseCount = 0;
    pacman.score.cherryCount = 0;
    pacman.score.pineappleCount = 0;
    pacman.speed = 1;
}

void gameReport(char room[n][m]) {
    printf("\n==============================\n");
    printf("======== game  report ========\n");
    printf("==============================\n\n");
    printf("=== room ===\n");
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%c", room[i][j]);
        }
        printf("\n");
    }
    printf("=== time ===\n%d\n", cycles);
    printf("=== pacman ===\nd:%d h:%d sX:%d sY:%d x:%d y:%d\n",
           pacman.direction,
           pacman.hearts,
           pacman.startPoint.x,
           pacman.startPoint.y,
           pacman.position.x,
           pacman.position.y
    );
    printf("=== blinky ===\nd:%d a:%d t:%d sX:%d sY:%d x:%d y:%d\n",
           blinky.direction,
           blinky.isAggresive,
           blinky.defensiveTimeLeft,
           blinky.startPoint.x,
           blinky.startPoint.y,
           blinky.position.x,
           blinky.position.y
    );
    printf("=== pinky ===\nd:%d a:%d t:%d sX:%d sY:%d x:%d y:%d\n",
           pinky.direction,
           pinky.isAggresive,
           pinky.defensiveTimeLeft,
           pinky.startPoint.x,
           pinky.startPoint.y,
           pinky.position.x,
           pinky.position.y
    );
    printf("=== clyde ===\nd:%d a:%d t:%d sX:%d sY:%d x:%d y:%d\n",
           clyde.direction,
           clyde.isAggresive,
           clyde.defensiveTimeLeft,
           clyde.startPoint.x,
           clyde.startPoint.y,
           clyde.position.x,
           clyde.position.y
    );
    printf("=== inky ===\nd:%d a:%d t:%d sX:%d sY:%d x:%d y:%d\n",
           inky.direction,
           inky.isAggresive,
           inky.defensiveTimeLeft,
           inky.startPoint.x,
           inky.startPoint.y,
           inky.position.x,
           inky.position.y
    );
}

///////////////////
/// methods end ///
///////////////////