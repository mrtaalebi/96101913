/////////////////
/// definitions ///
/// x is vertical ///
/// y is horizental ///
///////////////////////

#include <stdio.h>
#include <stdbool.h>

///////////////////////
/// constants start ///
///////////////////////

const int DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST = 5;

const enum Icons {
    EMPTY = '_',
    BLOCK = '#',
    CHEESE = '*',
    CHERRY = '^',
    PINEAPPLE = 'O',
} typedef Icons;

const double PACMAN_NORMAL_SPEED = 1,
        PACMAN_FAST_SPEED = 1.5,
        GHOST_AGGRESIVE_SPEED = 1,
        GHOST_DEFENSIVE_SPEED = 0.5;

const int CHEESE_VALUE = 1,
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
    int ghostsKilled;
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
    int isHero;
    int heroicTimeLeft;
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

char room[100][100];

int cycles;
int roomNumber;

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

void gameReport();

int runACycle();

void makeANewRoom();

void movePacmanToEat();

void pacmanBecomeAHero();

void pacmanHitAGhost(Ghost * ghost);

void movePacmanToGhosts();

bool areOnTheSamePosition(Point p1, Point p2);

int checkRemainingFoods();

void ghostBecomeDefensive(Ghost * ghost);

void restartRoomByPacmanDeath();

void restartAGhostByPacmanDeath(Ghost *ghost);

void log(char * s);


///////////////////
/// headers end ///
///////////////////

//////////////////
/// main start ///
//////////////////

int mod(int a, int m);

int main() {
    initialize();
    scanf("%d %d\n", &n, &m);
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




    int win = runACycle();
    printf("(%d,%d)\n", pacman.position.x, pacman.position.y);
    printf("%d\n", pacman.score.totalScore);
    if (win) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    //TODO: add standings

    //gameReport();

    return 0;
}

/// returns 0: continue current game
/// returns 1: game over launch standings
/// returns 2: loose a heart start from starting point
int runACycle() {
    cycles++;
    // move pacman to eat foods
    movePacmanToEat();
    // move pacman to hit ghosts
    movePacmanToGhosts();
    // eat and check winning condition
    int foodsLeft = checkRemainingFoods();
    if (foodsLeft == 0 && pacman.hearts > 0) {
        return 1;
    } else return 0;
}


////////////////
/// main end ///
////////////////

/////////////////////
/// methods start ///
/////////////////////

void log(char * s) {
    printf("%s", s);
}

void initialize() {
    cycles = 0;
    roomNumber = 1;
    pacman_init();
}

void pacman_init() {
    pacman.score.cheeseCount = 0;
    pacman.score.cherryCount = 0;
    pacman.score.pineappleCount = 0;
    pacman.speed = PACMAN_NORMAL_SPEED;
    pacman.isHero = 0;
    pacman.heroicTimeLeft = 0;
}

int checkRemainingFoods() {
    int foodsLeft = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char c = room[i][j];
            if (c == CHEESE || c == PINEAPPLE)
                foodsLeft++;
        }
    }
    return foodsLeft;
}

/// returns 0: stay
/// returns 1: pacman moved
void movePacmanToEat() {
    Point newPoint = pacman.position;
    switch (pacman.direction) {
        case UP:
            newPoint.x = mod(newPoint.x - 1, m);
            break;
        case RIGHT:
            newPoint.y = mod(newPoint.y + 1, n);
            break;
        case DOWN:
            newPoint.x = mod(newPoint.x + 1, m);
            break;
        case LEFT:
            newPoint.y = mod(newPoint.y - 1, n);
            break;
    }
    Icons nextStep = (Icons) room[newPoint.x][newPoint.y];
    if (nextStep != BLOCK) {
        pacman.position = newPoint;
    }
    Icons pacmanOn = (Icons) room[pacman.position.x][pacman.position.y];
    switch (pacmanOn) {
        case CHEESE:
            room[pacman.position.x][pacman.position.y] = EMPTY;
            pacman.score.cheeseCount++;
            pacman.score.totalScore += CHEESE_VALUE;
            break;
        case CHERRY:
            room[pacman.position.x][pacman.position.y] = EMPTY;
            pacman.score.cherryCount++;
            pacman.score.totalScore += CHERRY_VALUE;
            break;
        case PINEAPPLE:
            room[pacman.position.x][pacman.position.y] = EMPTY;
            pacman.score.pineappleCount++;
            pacmanBecomeAHero();
            break;
        default:
            break;
    }
}

int mod(int a, int m) {
    if (m > 0) {
        if (a >= 0) return a % m;
        else return m + a % m;
    } else return -1000000000;
}

void pacmanBecomeAHero() {
    pacman.isHero = 1;
    pacman.heroicTimeLeft = 10;
    pacman.speed = PACMAN_FAST_SPEED;
    ghostBecomeDefensive(&blinky);
    ghostBecomeDefensive(&pinky);
    ghostBecomeDefensive(&clyde);
    ghostBecomeDefensive(&inky);
}

void ghostBecomeDefensive(Ghost * ghost) {
    ghost->speed = GHOST_DEFENSIVE_SPEED;
    ghost->isAggresive = 0;
    ghost->defensiveTimeLeft = pacman.heroicTimeLeft;
}


/// returns sum of the return values of pacmanHitAGhost
void movePacmanToGhosts() {
    Point point = pacman.position;

    if (areOnTheSamePosition(point, blinky.position) && !blinky.isAggresive) {
        pacmanHitAGhost(&blinky);
    }
    if (areOnTheSamePosition(point, pinky.position) && !pinky.isAggresive) {
        pacmanHitAGhost(&pinky);
    }
    if (areOnTheSamePosition(point, clyde.position) && !clyde.isAggresive) {
        pacmanHitAGhost(&clyde);
    }
    if (areOnTheSamePosition(point, inky.position) && !inky.isAggresive) {
        pacmanHitAGhost(&inky);
    }

    if (areOnTheSamePosition(point, blinky.position) && blinky.isAggresive) {
        pacmanHitAGhost(&blinky);
    }
    if (areOnTheSamePosition(point, pinky.position) && pinky.isAggresive) {
        pacmanHitAGhost(&pinky);
    }
    if (areOnTheSamePosition(point, clyde.position) && clyde.isAggresive) {
        pacmanHitAGhost(&clyde);
    }
    if (areOnTheSamePosition(point, inky.position) && inky.isAggresive) {
        pacmanHitAGhost(&inky);
    }
}

bool areOnTheSamePosition(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

/// returns +10: aggresive ghost
/// returns -1: defensive ghost
void pacmanHitAGhost(Ghost * ghost) {
    if (!ghost->isAggresive) {
        pacman.score.ghostsKilled++;
        pacman.score.totalScore += GHOST_EAT_VALUE;
        ghost->position = ghost->startPoint;
        ghost->isAggresive = 1;
        ghost->defensiveTimeLeft = -1 * DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST;
        ghost->speed = GHOST_AGGRESIVE_SPEED;
    } else {
        restartRoomByPacmanDeath();
    }
}

void restartRoomByPacmanDeath() {
    pacman.hearts--;
    pacman.position = pacman.startPoint;
    pacman.heroicTimeLeft = 0;
    pacman.speed = PACMAN_NORMAL_SPEED;
    pacman.isHero = 0;
    restartAGhostByPacmanDeath(&blinky);
    restartAGhostByPacmanDeath(&pinky);
    restartAGhostByPacmanDeath(&clyde);
    restartAGhostByPacmanDeath(&inky);
}

void restartAGhostByPacmanDeath(Ghost *ghost) {
    ghost->speed = GHOST_AGGRESIVE_SPEED;
    ghost->position = ghost->startPoint;
    ghost->isAggresive = 1;
    ghost->defensiveTimeLeft = 0;
}

void makeANewRoom() {

}

void gameReport() {
    printf("\n==============================\n");
    printf("======== game  report ========\n");
    printf("==============================\n\n");
    printf("=== room ===\n");
    char out[n][m];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            out[i][j] = room[i][j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char *s = NULL;
            switch (room[i][j]) {
                case '_':
                    s = "░";
                    break;
                case '#':
                    s = "█";
                    break;
                case '*':
                    s = "▪";
                    break;
                case '^':
                    s = "♪";
                    break;
                case 'O':
                    s = "●";
                    break;
                case 'C':
                    s = "C";
                    break;
                case 'B':
                    s = "B";
                    break;
                case 'P':
                    s = "P";
                    break;
                case 'L':
                    s = "L";
                    break;
                case 'I':
                    s = "I";
                    break;
                default:
                    break;
            }
            printf("%s\t", s);
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
