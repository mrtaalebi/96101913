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

void movePacmanToEat(Point point, Direction direction);


void pacmanBecomeAHero();

int pacmanHitAGhost(Ghost * ghost);


///////////////////
/// headers end ///
///////////////////

//////////////////
/// main start ///
//////////////////

int movePacmanToGhosts(Point point);

bool areOnTheSamePosition(Point p1, Point p2);

int checkRemaningFoodsAndScorePacman() ;

void log(char * s) ;

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

    /// gameReport();

    /*
    int gameOver = 0;
    while (!gameOver) {
        int status = runACycle();
        switch (status) {
            case 0:
                break;
            case 1:
                gameOver = 1;
                break;
            case 2:
                pacman.position = pacman.startPoint;
                blinky.position = blinky.startPoint;
                pinky.position = pinky.startPoint;
                clyde.position = clyde.startPoint;
                inky.position = inky.startPoint;
                break;
            case 3:
                makeANewRoom();
                break;
            default:
                return 100;
        }
    }
    */


    int status = runACycle();
    if (status == 1 || status == 2) {
        pacman.position = pacman.startPoint;
        blinky.position = blinky.startPoint;
        pinky.position = pinky.startPoint;
        clyde.position = clyde.startPoint;
        inky.position = inky.startPoint;
    }
    printf("(%d,%d)\n", pacman.position.x, pacman.position.y);
    int foodsLeft = checkRemaningFoodsAndScorePacman();
    printf("%d\n", pacman.score.totalScore);
    if (foodsLeft == 0) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    //TODO: add standings

    /// gameReport();

    return 0;
}

void makeANewRoom() {

}

/// returns 0: continue current game
/// returns 1: game over launch standings
/// returns 2: loose a heart start from starting point
int runACycle() {
    // move pacman to eat foods
    movePacmanToEat(pacman.position, pacman.direction);
    // move pacman to hit ghosts
    int whatHappensMovingPacmanToGhosts = movePacmanToGhosts(pacman.position);
    if (whatHappensMovingPacmanToGhosts > 0) {
        pacman.hearts--;
        if (pacman.hearts == 0) return 1;
        else return 2;
    } else {
        return 2;
    }
}

int checkRemaningFoodsAndScorePacman() {
    int foodsLeft = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char c = room[i][j];
            if (c == CHEESE || c == PINEAPPLE)
                foodsLeft++;
        }
    }

    // giving score
    pacman.score.totalScore +=
            pacman.score.cherryCount * CHERRY_VALUE +
            pacman.score.cheeseCount * CHESSE_VALUE +
            pacman.score.ghostsKilled * GHOST_EAT_VALUE;

    return foodsLeft;
}

/// returns 0: stay
/// returns 1: pacman moved
void movePacmanToEat(Point point, Direction direction) {
    Point result = point;
    switch (direction) {
        case UP:
            result.x = (point.x - 1) % m;
            break;
        case RIGHT:
            result.y = (point.y + 1) % n;
            break;
        case DOWN:
            result.x = (point.x + 1) % m;
            break;
        case LEFT:
            result.y = (point.y - 1) % n;
            break;
    }
    Icons nextStep = (Icons) room[result.x][result.y];
    if (nextStep != BLOCK)
        switch (nextStep) {
            case CHEESE:
                room[result.x][result.y] = EMPTY;
                pacman.score.cheeseCount++;
                break;
            case CHERRY:
                room[result.x][result.y] = EMPTY;
                pacman.score.cherryCount++;
                break;
            case PINEAPPLE:
                room[result.x][result.y] = EMPTY;
                pacman.score.pineappleCount++;
                pacmanBecomeAHero();
                break;
            default:
                break;
        }
    pacman.position.x = result.x;
    pacman.position.y = result.y;
}

void pacmanBecomeAHero() {
    pacman.isHero = 1;
    pacman.heroicTimeLeft = 10;
    pacman.speed = PACMAN_FAST_SPEED;
    blinky.speed = GHOST_DEFENSIVE_SPEED;
    pinky.speed = GHOST_DEFENSIVE_SPEED;
    clyde.speed = GHOST_DEFENSIVE_SPEED;
    inky.speed = GHOST_DEFENSIVE_SPEED;
}


/// returns return values of pacmanHitAGhost
int movePacmanToGhosts(Point point) {
    int loseCondition = 0;
    if (areOnTheSamePosition(point, blinky.position)) {
        loseCondition += pacmanHitAGhost(&blinky);
    }
    if (areOnTheSamePosition(point, pinky.position)) {
        loseCondition += pacmanHitAGhost(&pinky);
    }
    if (areOnTheSamePosition(point, clyde.position)) {
        loseCondition += pacmanHitAGhost(&clyde);
    }
    if (areOnTheSamePosition(point, inky.position)) {
        loseCondition += pacmanHitAGhost(&inky);
    }
    return loseCondition;
}

bool areOnTheSamePosition(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}


/// returns +1: aggresive ghost
/// returns -1: defensive ghost
int pacmanHitAGhost(Ghost * ghost) {
    if (!ghost->isAggresive) {
        pacman.score.ghostsKilled++;
        ghost->position = ghost->startPoint;
        ghost->isAggresive = 1;
        ghost->defensiveTimeLeft = -1 * DELAY_MADE_WHEN_PACMAN_KILLS_A_GHOST;
        ghost->speed = GHOST_AGGRESIVE_SPEED;
        return -1;
    } else {
        return +1;
    }
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
    pacman.speed = 1;
    pacman.isHero = 0;
    pacman.heroicTimeLeft = 0;
}

void gameReport() {
    printf("\n==============================\n");
    printf("======== game  report ========\n");
    printf("==============================\n\n");
    printf("=== room ===\n");
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