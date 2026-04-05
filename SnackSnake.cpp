/*
 * SNAKE HUNT
 * Jogo da Cobra com Modos: Easy, Normal, Hard
 * + BFS Automatico, Power-Ups, Rankings por dificuldade
 *
 * Compile: gcc SnakeHunt.cpp -o SnakeHunt.exe
 * Requer: PowerShell ou Windows Terminal (UTF-8)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

/* ===================== DIMENSOES ===================== */
#define WIDTH          42
#define HEIGHT         22
#define TOP_OFFSET      2

#define ARENA_L         1
#define ARENA_R        (ARENA_L + WIDTH * 2)
#define PANEL_X        (ARENA_R + 3)
#define PANEL_W        22
#define CON_COLS       (PANEL_X + PANEL_W + 2)
#define CON_ROWS       28

/* ===================== JOGO ===================== */
#define FRUIT_BASE      4
#define FRUIT_EXTRA    10
#define FRUIT_TOTAL    (FRUIT_BASE + FRUIT_EXTRA)
#define MAX_SNAKE      900
#define MAX_RECORDS     5
#define RECORD_FILE_EASY   "ranking_easy.txt"
#define RECORD_FILE_NORMAL "ranking_normal.txt"
#define RECORD_FILE_HARD   "ranking_hard.txt"

#define SPEED_BASE     150
#define SPEED_MIN       45
#define SPEED_STEP      15
#define PTS_LEVEL      150
#define PTS_POWERUP    150
#define PTS_DIAMOND     50
#define PTS_APPLE       10
#define DIAMOND_TTL     15

#define OBSTACLE_COUNT  20

/* ===================== BFS ===================== */
#define BFS_MAX  (WIDTH * HEIGHT)

/* ===================== EMOJIS UTF-8 ===================== */
#define SYM_HEAD       "\xF0\x9F\x9F\xA0"
#define SYM_HEAD_AUTO  "\xF0\x9F\x98\x8E"
#define SYM_BODY       "\xF0\x9F\x9F\xA7"
#define SYM_BODY_PU    "\xF0\x9F\x94\xB5"
#define SYM_PIZZA      "\xF0\x9F\x8D\x95"
#define SYM_HAMBURGUER "\xF0\x9F\x8D\x94"
#define SYM_CAKE       "\xF0\x9F\x8E\x82"
#define SYM_POWERUP    "\xE2\xAD\x90"
#define SYM_SKULL      "\xF0\x9F\x92\x80"
#define SYM_TROPHY     "\xF0\x9F\x8F\x86"
#define SYM_CROWN      "\xF0\x9F\x91\x91"
#define SYM_FIRE       "\xF0\x9F\x94\xA5"
#define SYM_CTRL       "\xF0\x9F\x8E\xAE"
#define SYM_ROBOT      "\xF0\x9F\xA4\x96"
#define SYM_SNAKE      "\xF0\x9F\x90\x8D"
#define SYM_STAR       "\xE2\xAD\x90"
#define SYM_HEART      "\xE2\x9D\xA4"
#define SYM_ZAP        "\xE2\x9A\xA1"
#define SYM_CLOCK      "\xE2\x8F\xB1"
#define SYM_CHART      "\xF0\x9F\x93\x88"
#define SYM_MEDAL      "\xF0\x9F\x8F\x85"
#define SYM_BOMB       "\xF0\x9F\x92\xA3"
#define SYM_FIRE       "\xF0\x9F\x94\xA5"
#define SYM_EASY       "\xF0\x9F\x90\xA2"
#define SYM_NORMAL     "\xF0\x9F\x8E\xAF"
#define SYM_HARD       "\xF0\x9F\x94\xA5"

/* Bordas box-drawing */
#define BOX_TL   "\xE2\x94\x8C"
#define BOX_TR   "\xE2\x94\x90"
#define BOX_BL   "\xE2\x94\x94"
#define BOX_BR   "\xE2\x94\x98"
#define BOX_H    "\xE2\x94\x80"
#define BOX_V    "\xE2\x94\x82"
#define BOX_TLH  "\xE2\x95\x94"
#define BOX_TRH  "\xE2\x95\x97"
#define BOX_BLH  "\xE2\x95\x9A"
#define BOX_BRH  "\xE2\x95\x9D"
#define BOX_DH   "\xE2\x95\x90"
#define BOX_DV   "\xE2\x95\x91"
#define BOX_LM   "\xE2\x95\xA0"
#define BOX_RM   "\xE2\x95\xA3"
#define BOX_LMS  "\xE2\x94\x9C"
#define BOX_RMS  "\xE2\x94\xA4"
#define BLOCK    "\xE2\x96\x93"
#define HALF_L   "\xE2\x96\x8C"
#define HALF_R   "\xE2\x96\x90"
#define DOT_MID  "\xC2\xB7"

/* ===================== CORES ===================== */
#define C_RESET     7
#define C_BORDER   11
#define C_PANEL    11
#define C_HDR      14
#define C_VAL      15
#define C_SCORE    14
#define C_LEVEL    10
#define C_SPEED    12
#define C_TITLE    14
#define C_SELECT   14
#define C_NORMAL    7
#define C_DIM       8
#define C_GAMEOVER 12
#define C_GOOD     10
#define C_FLASH    15
#define C_PU       13
#define C_AUTO      9
#define C_OBSTACLE  4

#define C_SCORE_LABEL   11
#define C_SCORE_VAL     15
#define C_LEVEL_LABEL    6
#define C_LEVEL_VAL     14
#define C_TIME_LABEL     3
#define C_TIME_VAL      11
#define C_PU_LABEL      13
#define C_PU_VAL        13
#define C_KEY_BG         8
#define C_KEY_FG        15
#define C_SEP            8
#define C_ACCENT        14
#define C_BORDER_PANEL   6

/* ===================== TIPOS ===================== */
typedef struct Node {
    int x, y;
    struct Node *next, *prev;
} Node;

typedef struct {
    int x, y;
    int type;
    int active;
} Fruit;

typedef struct {
    char name[32];
    int  score;
} Record;

typedef enum { DIR_UP=0, DIR_RIGHT, DIR_DOWN, DIR_LEFT } Direction;

typedef enum {
    ST_MENU=0, ST_PLAYING, ST_RANKING, ST_GAMEOVER, ST_EXIT
} State;

typedef enum {
    MODE_EASY=0, MODE_NORMAL, MODE_HARD
} GameMode;

typedef struct { int x, y; } Pt;

/* ===================== GLOBAIS ===================== */
HANDLE hConsole;

Node   *snakeHead = NULL, *snakeTail = NULL;
int     snakeLen  = 0;

#define HASH_H (HEIGHT + TOP_OFFSET + 4)
int occupancy[HASH_H][WIDTH + 2];

Fruit fruits[FRUIT_TOTAL];
int   diamondActive = 0;
time_t diamondTime;

/* Obstáculos (modo Hard) */
typedef struct { int x, y; } ObstaclePos;
ObstaclePos obstacles[OBSTACLE_COUNT];
int obstacleCount = 0;

Direction dir     = DIR_RIGHT;
Direction nextDir = DIR_RIGHT;
int score         = 0;
int level         = 1;
int speedMs       = SPEED_BASE;
int gameOver      = 0;
int paused        = 0;
int autoMode      = 0;
time_t startTime;
time_t pauseStart;
int    pausedSec  = 0;

int lastMilestonePU   = 0;
int lastMilestoneLVL  = 0;
int powerUpActive     = 0;
time_t powerUpStart;

int menuIndex    = 0;
int modeIndex    = 0;   /* seleção de modo no menu */
int goIndex      = 0;
int rankingTab   = 0;   /* aba de ranking: 0=Easy, 1=Normal, 2=Hard */

GameMode gameMode = MODE_NORMAL;
char playerName[32] = "";

Record ranking[MAX_RECORDS];
int rankCount    = 0;

Pt   bfsQueue[BFS_MAX];
int  bfsParentX[HEIGHT][WIDTH + 2];
int  bfsParentY[HEIGHT][WIDTH + 2];
int  bfsVisited[HEIGHT][WIDTH + 2];
int  bfsDirX[4] = { 0, 1,  0, -1 };
int  bfsDirY[4] = {-1, 0,  1,  0 };
Direction bfsNextDir = DIR_RIGHT;

int needRedrawMenu   = 1;
int needRedrawPanel  = 1;
int needRedrawArena  = 1;
int needRedrawGameOver = 1;
int needRedrawRanking  = 1;

/* ===================== UTILITARIOS ===================== */
void setColor(int c) { SetConsoleTextAttribute(hConsole, c); }

void gotoxy(int x, int y) {
    COORD p = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, p);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(hConsole, &ci);
}

void showCursor() {
    CONSOLE_CURSOR_INFO ci = {1, TRUE};
    SetConsoleCursorInfo(hConsole, &ci);
}

void clearScreen() {
    COORD c = {0,0};
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', size, c, &written);
    FillConsoleOutputAttribute(hConsole, C_RESET, size, c, &written);
    SetConsoleCursorPosition(hConsole, c);
}

void printCenter(int l, int r, int y, int color, const char *s) {
    int len = (int)strlen(s);
    int x   = l + (r - l - len) / 2;
    if (x < l) x = l;
    gotoxy(x, y);
    setColor(color);
    printf("%s", s);
}

/* ===================== LISTA ENCADEADA ===================== */
void insertHead(int x, int y) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) return;
    n->x = x; n->y = y;
    n->next = snakeHead; n->prev = NULL;
    if (snakeHead) snakeHead->prev = n;
    snakeHead = n;
    if (!snakeTail) snakeTail = n;
    occupancy[y][x] = 1;
    snakeLen++;
}

void removeTail() {
    if (!snakeTail) return;
    occupancy[snakeTail->y][snakeTail->x] = 0;
    Node *tmp = snakeTail;
    snakeTail = snakeTail->prev;
    if (snakeTail) snakeTail->next = NULL;
    else snakeHead = NULL;
    free(tmp);
    snakeLen--;
}

void freeSnake() {
    Node *t = snakeHead;
    while (t) {
        Node *nx = t->next;
        free(t);
        t = nx;
    }
    snakeHead = snakeTail = NULL;
    snakeLen = 0;
}

/* ===================== LANCHES ===================== */
int arenaX(int gx) { return ARENA_L + gx * 2; }
int arenaY(int gy) { return TOP_OFFSET + 1 + gy; }

void eraseFruitAt(int gx, int gy) {
    gotoxy(arenaX(gx), arenaY(gy));
    printf("  ");
}

void drawFruit(int i) {
    if (!fruits[i].active) return;
    gotoxy(arenaX(fruits[i].x), arenaY(fruits[i].y));
    if (fruits[i].type == 1) { setColor(12); printf("%s", SYM_PIZZA);   }
    if (fruits[i].type == 2) { setColor(14); printf("%s", SYM_CAKE); }
    if (fruits[i].type == 3) { setColor(11); printf("%s", SYM_HAMBURGUER); }
    setColor(C_RESET);
}

int isObstacleAt(int gx, int gy) {
    for (int i = 0; i < obstacleCount; i++)
        if (obstacles[i].x == gx && obstacles[i].y == gy) return 1;
    return 0;
}

void drawObstacle(int gx, int gy) {
    gotoxy(arenaX(gx), arenaY(gy));
    setColor(C_OBSTACLE);
    printf("%s ", SYM_BOMB);
    setColor(C_RESET);
}

void drawAllObstacles() {
    for (int i = 0; i < obstacleCount; i++)
        drawObstacle(obstacles[i].x, obstacles[i].y);
}

void spawnObstacles() {
    obstacleCount = 0;
    int mx = WIDTH / 2;
    int my = HEIGHT / 2;
    for (int i = 0; i < OBSTACLE_COUNT && obstacleCount < OBSTACLE_COUNT; i++) {
        int x, y, ok;
        int attempts = 0;
        do {
            ok = 1;
            x = 1 + rand() % (WIDTH - 2); 
            y = 1 + rand() % (HEIGHT - 2);
            if (abs(x - mx) <= 5 && abs(y - my) <= 2) { ok = 0; continue; }
            if (occupancy[arenaY(y)][x] == 1) { ok = 0; continue; }
            if (isObstacleAt(x, y)) { ok = 0; continue; }
            attempts++;
            if (attempts > 200) break;
        } while (!ok);
        if (ok && attempts <= 200) {
            obstacles[obstacleCount].x = x;
            obstacles[obstacleCount].y = y;
            obstacleCount++;
        }
    }
}

void spawnFruit(int i, int type) {
    int x, y, tries = 0;
    do {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
        if (++tries > 2000) return;
    } while (occupancy[arenaY(y)][x] != 0 ||
             occupancy[TOP_OFFSET + 1 + y][x] != 0 ||
             isObstacleAt(x, y));

    fruits[i].x = x; fruits[i].y = y;
    fruits[i].type = type; fruits[i].active = 1;
    occupancy[arenaY(y)][x] = type + 1;
    drawFruit(i);
}

void removeFruitSlot(int i) {
    if (!fruits[i].active) return;
    occupancy[arenaY(fruits[i].y)][fruits[i].x] = 0;
    eraseFruitAt(fruits[i].x, fruits[i].y);
    fruits[i].active = 0;
}

void spawnInitialFruits() {
    for (int i = 0; i < FRUIT_BASE; i++) spawnFruit(i, 1);
}

/* ===================== BFS ===================== */
int bfs() {
    memset(bfsVisited, 0, sizeof(bfsVisited));
    memset(bfsParentX, -1, sizeof(bfsParentX));
    memset(bfsParentY, -1, sizeof(bfsParentY));

    /* Encontra a fruta mais próxima (não só a primeira) */
    int bestDist = BFS_MAX + 1;
    int tx = -1, ty = -1;  /* coordenadas LÓGICAS da melhor fruta */

    /* Cabeça em coordenadas lógicas */
    int sx = snakeHead->x;
    int sy_screen = snakeHead->y;  /* y de tela */

    for (int fi = 0; fi < FRUIT_TOTAL; fi++) {
        if (!fruits[fi].active) continue;
        int fx = fruits[fi].x;
        int fy = fruits[fi].y;  /* y lógico da fruta */
        int dist = abs(fx - sx) + abs(fy - (sy_screen - TOP_OFFSET - 1));
        if (dist < bestDist) {
            bestDist = dist;
            tx = fx;
            ty = fy;  /* salva em y LÓGICO */
        }
    }
    if (tx < 0) return 0;

    /* BFS em coordenadas: x lógico, y lógico (0..HEIGHT-1) */
    int sy = sy_screen - TOP_OFFSET - 1;  /* converte cabeça p/ lógico */

    int front = 0, rear = 0;
    bfsQueue[rear].x = sx; bfsQueue[rear].y = sy; rear++;
    bfsVisited[sy][sx] = 1;
    bfsParentX[sy][sx] = -1; bfsParentY[sy][sx] = -1;

    while (front < rear) {
        Pt cur = bfsQueue[front++];

       if (cur.x == tx && cur.y == ty) {
            /* Reconstrói caminho */
            int cx = tx, cy = ty;
            int steps = 0;
            int maxSteps = WIDTH * HEIGHT;

            while ((bfsParentX[cy][cx] != sx || bfsParentY[cy][cx] != sy)
                   && steps < maxSteps) {
                int px = bfsParentX[cy][cx];
                int py = bfsParentY[cy][cx];
                if (px < 0 || py < 0) break;
                cx = px; cy = py;
                steps++;
            }

            if (steps >= maxSteps) return 0;

            if      (cy < sy) bfsNextDir = DIR_UP;
            else if (cy > sy) bfsNextDir = DIR_DOWN;
            else if (cx > sx) bfsNextDir = DIR_RIGHT;
            else              bfsNextDir = DIR_LEFT;
            return 1;
        }

        for (int d = 0; d < 4; d++) {
            int nx = cur.x + bfsDirX[d];
            int ny = cur.y + bfsDirY[d];

            if (nx < 0 || nx >= WIDTH) continue;
            if (ny < 0 || ny >= HEIGHT) continue;
            if (bfsVisited[ny][nx]) continue;

            /* Converte para y de tela para checar occupancy */
            int ny_screen = ny + TOP_OFFSET + 1;
            if (occupancy[ny_screen][nx] == 1) continue;
            if (isObstacleAt(nx, ny)) continue;

            bfsVisited[ny][nx] = 1;
            bfsParentX[ny][nx] = cur.x;
            bfsParentY[ny][nx] = cur.y;
            bfsQueue[rear].x = nx;
            bfsQueue[rear].y = ny;
            rear++;
        }
    }
    return 0;
}
/* ===================== RENDER: ARENA ===================== */
void drawBorder() {
    setColor(12);

    gotoxy(ARENA_L - 1, TOP_OFFSET);
    printf("%s", BOX_TLH);
    for (int i = 0; i < WIDTH; i++) printf("%s%s", BOX_DH, BOX_DH);
    printf("%s", BOX_TRH);

    for (int y = 1; y <= HEIGHT; y++) {
        gotoxy(ARENA_L - 1, TOP_OFFSET + y);
        printf("%s", BOX_DV);
        gotoxy(ARENA_L + WIDTH * 2, TOP_OFFSET + y);
        printf("%s", BOX_DV);
    }

    gotoxy(ARENA_L - 1, TOP_OFFSET + HEIGHT + 1);
    printf("%s", BOX_BLH);
    for (int i = 0; i < WIDTH; i++) printf("%s%s", BOX_DH, BOX_DH);
    printf("%s", BOX_BRH);

    setColor(C_RESET);
}

void renderSnake() {
    Node *cur = snakeHead;
    int   i   = 0;
    while (cur) {
        gotoxy(arenaX(cur->x), cur->y);
        if (i == 0) {
            if (autoMode) { setColor(C_AUTO);  printf("%s", SYM_HEAD_AUTO); }
            else          { setColor(C_GOOD);  printf("%s", SYM_HEAD);      }
        } else {
            if (powerUpActive) {
                int ciclo = (i + (int)(clock() / 120)) % 3;
                setColor(ciclo == 0 ? 13 : ciclo == 1 ? 15 : 11);
                printf("%s", SYM_BODY_PU);
            } else {
                setColor(i < 4 ? 10 : 2);
                printf("%s", SYM_BODY);
            }
        }
        cur = cur->next;
        i++;
    }
    setColor(C_RESET);
}

/* ===================== RENDER: PAINEL LATERAL ===================== */
static void panelSep(int py) {
    setColor(C_SEP);
    gotoxy(PANEL_X + 1, py);
    for (int i = 0; i < PANEL_W; i++) printf("%s", BOX_H);
    setColor(C_RESET);
}

static void panelKey(int px, int py, const char *key, const char *desc) {
    setColor(C_ACCENT);
    gotoxy(px, py);
    printf("[");
    setColor(C_KEY_FG);
    printf("%s", key);
    setColor(C_ACCENT);
    printf("]");
    setColor(C_DIM);
    printf(" %s", desc);
    setColor(C_RESET);
}

void drawPanelBox() {
    setColor(C_BORDER_PANEL);

    gotoxy(PANEL_X, TOP_OFFSET);
    printf("%s", BOX_TLH);
    for (int i = 0; i < PANEL_W; i++) printf("%s", BOX_DH);
    printf("%s", BOX_TRH);

    for (int y = TOP_OFFSET + 1; y <= TOP_OFFSET + HEIGHT; y++) {
        gotoxy(PANEL_X, y);               printf("%s", BOX_DV);
        gotoxy(PANEL_X + PANEL_W + 1, y); printf("%s", BOX_DV);
    }

    gotoxy(PANEL_X, TOP_OFFSET + HEIGHT + 1);
    printf("%s", BOX_BLH);
    for (int i = 0; i < PANEL_W; i++) printf("%s", BOX_DH);
    printf("%s", BOX_BRH);

    gotoxy(PANEL_X, TOP_OFFSET + 2);
    printf("%s", BOX_LM);
    for (int i = 0; i < PANEL_W; i++) printf("%s", BOX_DH);
    printf("%s", BOX_RM);

    setColor(C_RESET);
}

void drawControls() {
    /* Linha abaixo da borda inferior da arena */
    int cy = TOP_OFFSET + HEIGHT + 2;
    int px = ARENA_L - 1;

    setColor(C_DIM);
    gotoxy(px, cy);

    /* Modo atual */
    if (autoMode) { setColor(C_AUTO);  printf("%s AUTO   ", SYM_ROBOT);  }
    else          { setColor(C_GOOD);  printf("%s MANUAL ", SYM_CTRL);   }

    setColor(C_ACCENT); printf("[");
    setColor(C_KEY_FG); printf("P");
    setColor(C_ACCENT); printf("]");
    setColor(C_DIM);
    printf(paused ? " Retomar  " : " Pausar   ");

    setColor(C_ACCENT); printf("[");
    setColor(C_KEY_FG); printf("T");
    setColor(C_ACCENT); printf("]");
    setColor(C_DIM);
    printf(autoMode ? " MANUAL   " : " AUTO     ");

    setColor(C_ACCENT); printf("[");
    setColor(C_KEY_FG); printf("Q");
    setColor(C_ACCENT); printf("]");
    setColor(C_DIM);
    printf(" Menu");

    setColor(C_RESET);
}

void updatePanel() {
    int py = TOP_OFFSET + 1;

    /* TÍTULO DO PAINEL */
    setColor(C_ACCENT);
    gotoxy(PANEL_X + 2, py);
    printf("%s", SYM_PIZZA);
    setColor(15);
    printf(" SNACK");
    setColor(C_ACCENT);
    printf(" SNAKE");
    setColor(C_RESET);

    py = TOP_OFFSET + 3;

    /* MODO + JOGADOR */
    setColor(C_DIM);
    gotoxy(PANEL_X + 2, py);
    const char *modeName = (gameMode==MODE_EASY)?"EASY":(gameMode==MODE_NORMAL)?"NORMAL":"HARD";
    int modeColor = (gameMode==MODE_EASY)?10:(gameMode==MODE_NORMAL)?14:12;
    printf("DIFICULDADE: ");
    setColor(modeColor);
    printf("%-8s", modeName);
    setColor(C_DIM);
    gotoxy(PANEL_X + 2, py+1);
    printf("%-18.18s", playerName);

    py += 2;
    panelSep(py);
    py++;

    /* PONTUAÇÃO */
    setColor(C_SCORE_LABEL);
    gotoxy(PANEL_X + 2, py);
    printf("%s PONTOS", SYM_TROPHY);

    setColor(C_SEP);
    gotoxy(PANEL_X + 2, py + 1);
    printf("  ");

    setColor(C_SCORE_VAL);
    gotoxy(PANEL_X + 4, py + 1);
    printf("%-14d", score);

    py += 2;
    panelSep(py);
    py++;

    /* NÍVEL */
    setColor(C_LEVEL_LABEL);
    gotoxy(PANEL_X + 2, py);
    printf("%s NIVEL", SYM_MEDAL);

    setColor(C_LEVEL_VAL);
    gotoxy(PANEL_X + 4, py + 1);
    int stars = ((level - 1) % 5) + 1;  /* 1?1, 2?2, 3?3, 4?4, 5?5, 6?1, 7?2... */
    for (int s = 0; s < 5; s++) {
        if (s < stars) { setColor(C_LEVEL_VAL); }
        else           { setColor(C_SEP); }
    }
    setColor(C_LEVEL_VAL);
    printf("  %d", level);

    py += 2;
    panelSep(py);
    py++;

    /* TEMPO */
    time_t elapsed = time(NULL) - startTime - pausedSec;
    int min = (int)elapsed / 60;
    int sec = (int)elapsed % 60;
    char timeBuf[16];
    sprintf(timeBuf, "%02d : %02d", min, sec);

    setColor(C_TIME_LABEL);
    gotoxy(PANEL_X + 2, py);
    printf("%s TEMPO", SYM_CLOCK);

    setColor(C_TIME_VAL);
    gotoxy(PANEL_X + 4, py + 1);
    printf("%-14s", timeBuf);

    py += 2;
    panelSep(py);
    py++;

    /* POWER-UP */
    setColor(C_PU_LABEL);
    gotoxy(PANEL_X + 2, py);
    printf("%s POWER-UP", SYM_ZAP);

    gotoxy(PANEL_X + 4, py + 1);
    if (powerUpActive) {
        int rem = 10 - (int)difftime(time(NULL), powerUpStart);
        if (rem < 0) rem = 0;
        int filled = rem;
        setColor(13);
        for (int b = 0; b < 10; b++) {
            if (b < filled) printf("\xE2\x96\x88");
            else { setColor(C_SEP); printf("\xE2\x96\x91"); setColor(13); }
        }
        setColor(C_PU_VAL);
        printf(" %ds", rem);
    } else {
        setColor(C_SEP);
        printf("%-16s", "inativo");
    }

    py += 2;
    panelSep(py);
    py++;
    
    /* LEGENDA DE ITENS */
    setColor(C_DIM);
	gotoxy(PANEL_X + 2, py);
	printf("ITENS");
	py++;
	setColor(12);
	gotoxy(PANEL_X + 2, py);
	printf("%s Pizza   ", SYM_PIZZA);
	setColor(C_SCORE_VAL);
	printf("+%d pts", PTS_APPLE);
	py++;
	setColor(11);
	gotoxy(PANEL_X + 2, py);
	printf("%s Burger  ", SYM_HAMBURGUER);
	setColor(C_SCORE_VAL);
	printf("+%d pts", PTS_DIAMOND);
	py++;
	setColor(14);
	gotoxy(PANEL_X + 2, py);
	printf("%s BOLO", SYM_CAKE);
	setColor(C_DIM);
	printf(" X2 PIZZA");
	py++;
	panelSep(py); py++;
}

   
/* ===================== RENDER: TELA INICIAL (SNACK SNAKE) ===================== */
void renderMenu() {
    if (!needRedrawMenu) return;
    clearScreen();

    int midX = CON_COLS / 2;

    /* ---- Arte ASCII grande "SNACK SNAKE" ---- */
    /* Linha decorativa topo */
    setColor(14);
    gotoxy(2, 0);
    for (int i = 0; i < CON_COLS - 4; i++) printf("%s", BOX_DH);

	/* Arte em bloco */
	/* S N A C K   S N A K E */
	setColor(12); /* verde brilhante */
	gotoxy(25, 1);
	printf(" ___  _  _   _   ____  _  _      ___  _  _   _   _  _  ____");
	gotoxy(25, 2);
	printf("/ __)( \\( ) / \\ /  __)( )/ )    / __)( \\( ) / \\ ( )/ )( ___)");
	gotoxy(25, 3);
	printf("\\__ \\ )  ( / O \\| (__  )  (     \\__ \\ )  ( / O \\ )  (  )__) ");
	gotoxy(25, 4);
	printf("(___/(_)\\_)_/ \\_\\____)(_)\\_)    (___/(_)\\_)_/ \\_(_)\\_)(____)");
	
    /* Linha separadora dupla */
    setColor(14);
    gotoxy(2, 5);
    for (int i = 0; i < CON_COLS - 4; i++) printf("%s", BOX_DH);

    /* Decoração lateral com cobras */
    setColor(10);
    gotoxy(2, 1);  printf("%s", SYM_PIZZA);
    gotoxy(2, 2);  printf("%s", SYM_HAMBURGUER);
    gotoxy(2, 3);  printf("%s", SYM_PIZZA);
    gotoxy(2, 4);  printf("%s", SYM_HAMBURGUER);
    gotoxy(CON_COLS-4, 1);  printf("%s", SYM_PIZZA);
    gotoxy(CON_COLS-4, 2);  printf("%s", SYM_HAMBURGUER);
    gotoxy(CON_COLS-4, 3);  printf("%s", SYM_PIZZA);
    gotoxy(CON_COLS-4, 4);  printf("%s", SYM_HAMBURGUER);

    /* Subtítulo caçador */
    setColor(15);
    gotoxy(midX - 15, 5);
    printf("  %s  COMA - AUMENTE - DOMINE  %s  ", SYM_CAKE, SYM_CAKE);

    /* ---- Caixa do menu ---- */
    int ML = midX - 18, MR = midX + 18;
    int MT = 7, MB = 20;

    setColor(12);
    gotoxy(ML, MT); printf("%s", BOX_TLH);
    for (int i = ML+1; i < MR; i++) { gotoxy(i, MT); printf("%s", BOX_DH); }
    gotoxy(MR, MT); printf("%s", BOX_TRH);

    gotoxy(ML, MT+1); printf("%s", BOX_LM);
    for (int i = ML+1; i < MR; i++) { gotoxy(i, MT+1); printf("%s", BOX_DH); }
    gotoxy(MR, MT+1); printf("%s", BOX_RM);

    for (int y = MT+2; y < MB; y++) {
        gotoxy(ML, y); printf("%s", BOX_DV);
        gotoxy(MR, y); printf("%s", BOX_DV);
    }

    gotoxy(ML, MB); printf("%s", BOX_BLH);
    for (int i = ML+1; i < MR; i++) { gotoxy(i, MB); printf("%s", BOX_DH); }
    gotoxy(MR, MB); printf("%s", BOX_BRH);

    setColor(15);
    gotoxy(ML + 6, MT); printf("   %s MENU PRINCIPAL %s  ", SYM_PIZZA, SYM_PIZZA);

    const char *labels[] = {
        "      GAME NORMAL       ",
        "    GAME AUTOMATICO     ",
        "        PLACARES        ",
        "          SAIR          "
    };
    int cols[] = { 6, C_AUTO, C_HDR, C_DIM };

    for (int i = 0; i < 4; i++) {
        int oy = MT + 3 + i * 2;
        if (i == menuIndex) {
            setColor(cols[i]);
            gotoxy(ML + 2, oy);
            printf("%s [ %s ] %s", SYM_ZAP, labels[i], SYM_ZAP);
        } else {
            setColor(C_DIM);
            gotoxy(ML + 4, oy);
            printf("  %s  ", labels[i]);
        }
    }

    setColor(C_DIM);
    gotoxy(midX - 22, MB + 2);
    printf("Use SETAS para navegar  |  ENTER para confirmar");

    /* Linha decorativa rodapé */
    setColor(14);
    gotoxy(2, MB + 3);
    for (int i = 0; i < CON_COLS - 4; i++) printf("%s", BOX_DH);

    setColor(C_RESET);
    needRedrawMenu = 0;
}

/* ===================== TELA DE SELEÇÃO DE MODO ===================== */
void renderModeSelect() {
    clearScreen();

    int midX = CON_COLS / 2;

    setColor(14);
    gotoxy(midX - 14, 2);
    printf("%s  SELECIONE A DIFICULDADE  %s", SYM_HAMBURGUER, SYM_HAMBURGUER);

    setColor(14);
    gotoxy(4, 4);
    for (int i = 0; i < CON_COLS - 8; i++) printf("%s", BOX_DH);

    int ML = midX - 16, MR = midX + 16;
    int MT = 6, MB = 18;

    setColor(12);
    gotoxy(ML, MT); printf("%s", BOX_TLH);
    for (int i = ML+1; i < MR; i++) { gotoxy(i, MT); printf("%s", BOX_DH); }
    gotoxy(MR, MT); printf("%s", BOX_TRH);
    for (int y = MT+1; y < MB; y++) {
        gotoxy(ML, y); printf("%s", BOX_DV);
        gotoxy(MR, y); printf("%s", BOX_DV);
    }
    gotoxy(ML, MB); printf("%s", BOX_BLH);
    for (int i = ML+1; i < MR; i++) { gotoxy(i, MB); printf("%s", BOX_DH); }
    gotoxy(MR, MB); printf("%s", BOX_BRH);

    struct { const char *sym; const char *name; const char *desc; int color; } modes[3] = {
        { SYM_EASY,   "EASY",   "Sem aumento de velocidade",   10 },
        { SYM_NORMAL, "NORMAL", "Velocidade sobe a cada 5 lvl", 14 },
        { SYM_HARD,   "HARD",   "Obstaculos que mudam de lugar",12 }
    };

    for (int i = 0; i < 3; i++) {
        int oy = MT + 2 + i * 3;
        if (i == modeIndex) {
            setColor(modes[i].color);
            gotoxy(ML + 2, oy);
            printf("%s %s [ %s ]", SYM_ZAP, modes[i].sym, modes[i].name);
            setColor(modes[i].color);
            gotoxy(ML + 2, oy + 1);
            printf("%s", modes[i].desc);
        } else {
            setColor(C_DIM);
            gotoxy(ML + 4, oy);
            printf("%s  %s", modes[i].sym, modes[i].name);
            gotoxy(ML + 2, oy + 1);
            printf("%s", modes[i].desc);
        }
    }

    setColor(C_DIM);
    gotoxy(midX - 22, MB + 2);
    printf("Use SETAS para escolher  |  ENTER para confirmar");

    setColor(C_RESET);
}

/* ===================== TELA DE NOME DO JOGADOR ===================== */
void askPlayerName() {
    clearScreen();
    int midX = CON_COLS / 2;

    setColor(14);
    gotoxy(midX - 8, 3);
    printf("%s  SNACK SNAKE  %s", SYM_PIZZA, SYM_PIZZA);

    setColor(14);
    gotoxy(4, 5);
    for (int i = 0; i < CON_COLS - 8; i++) printf("%s", BOX_DH);

    const char *modeName = (gameMode==MODE_EASY)?"EASY":(gameMode==MODE_NORMAL)?"NORMAL":"HARD";
    int modeColor = (gameMode==MODE_EASY)?10:(gameMode==MODE_NORMAL)?14:12;

    setColor(C_DIM);
    gotoxy(midX - 10, 7);
    printf("Modo selecionado: ");
    setColor(modeColor);
    printf("%s", modeName);

    setColor(C_VAL);
    gotoxy(midX - 10, 10);
    printf("%s  Nome do Jogador:", SYM_CROWN);

    setColor(12);
    gotoxy(midX - 10, 11);
    for (int i = 0; i < 22; i++) printf("%s", BOX_DH);

    setColor(C_DIM);
    gotoxy(midX - 15, 13);
    printf("(Pressione ENTER para confirmar)");

    showCursor();
    gotoxy(midX - 10, 12);
    setColor(C_VAL);
    fflush(stdout);

    memset(playerName, 0, sizeof(playerName));
    if (fgets(playerName, sizeof(playerName), stdin)) {
        int l = (int)strlen(playerName);
        if (l > 0 && playerName[l-1]=='\n') playerName[l-1]='\0';
    }
    if (strlen(playerName) == 0) strcpy(playerName, "Jogador");
    hideCursor();
}

/* ===================== RENDER: RANKING ===================== */
const char* getRankingFile(int tab) {
    if (tab == 0) return RECORD_FILE_EASY;
    if (tab == 1) return RECORD_FILE_NORMAL;
    return RECORD_FILE_HARD;
}

void loadRanking(int tab) {
    rankCount = 0;
    FILE *f = fopen(getRankingFile(tab), "r");
    if (!f) return;
    while (rankCount < MAX_RECORDS &&
           fscanf(f, "%31s %d", ranking[rankCount].name, &ranking[rankCount].score) == 2)
        rankCount++;
    fclose(f);
}

void saveRanking(const char *name, int pts) {
    loadRanking((int)gameMode);
    if (rankCount < MAX_RECORDS) {
        strncpy(ranking[rankCount].name, name, 31);
        ranking[rankCount].score = pts;
        rankCount++;
    } else if (pts > ranking[rankCount-1].score) {
        strncpy(ranking[rankCount-1].name, name, 31);
        ranking[rankCount-1].score = pts;
    } else return;

    for (int i = 0; i < rankCount-1; i++)
        for (int j = 0; j < rankCount-i-1; j++)
            if (ranking[j].score < ranking[j+1].score) {
                Record tmp = ranking[j]; ranking[j] = ranking[j+1]; ranking[j+1] = tmp;
            }

    FILE *f = fopen(getRankingFile((int)gameMode), "w");
    if (f) {
        for (int i = 0; i < rankCount; i++)
            fprintf(f, "%s %d\n", ranking[i].name, ranking[i].score);
        fclose(f);
    }
}

void renderRanking() {
    if (!needRedrawRanking) return;
    needRedrawRanking = 0;

    clearScreen();
    int midX = CON_COLS / 2;

    setColor(14);
    gotoxy(midX - 11, 1);
    printf("%s  TOP %d  SNACK SNAKE  %s", SYM_TROPHY, MAX_RECORDS, SYM_TROPHY);

    /* Abas de ranking */
    const char *tabNames[] = { " EASY ", " NORMAL ", " HARD " };
    int tabColors[] = { 10, 14, 12 };
    int tabX = midX - 12;
    for (int t = 0; t < 3; t++) {
        if (t == rankingTab) {
            setColor(tabColors[t]);
            gotoxy(tabX, 3);
            printf("[%s]", tabNames[t]);
        } else {
            setColor(C_DIM);
            gotoxy(tabX, 3);
            printf(" %s ", tabNames[t]);
        }
        tabX += (int)strlen(tabNames[t]) + 3;
    }

    setColor(11);
    gotoxy(8, 5);
    for (int i = 0; i < CON_COLS - 16; i++) printf("%s", BOX_DH);

    loadRanking(rankingTab);
    const char *medals[] = { "\xF0\x9F\xA5\x87", "\xF0\x9F\xA5\x88", "\xE2\xAD\x90" };
    int colors[] = { 14, 7, 6, 8, 8 };

    for (int i = 0; i < rankCount; i++) {
        gotoxy(midX - 14, 8 + i * 2);
        setColor(colors[i < 5 ? i : 4]);
        if (i < 3) printf("%s  ", medals[i]);
        else printf("   %d.  ", i+1);
        printf("%-20s  %d pts", ranking[i].name, ranking[i].score);
    }
    if (rankCount == 0) {
        setColor(C_DIM);
        gotoxy(midX - 10, 11);
        printf("Nenhum recorde ainda...");
    }

    setColor(C_DIM);
    gotoxy(midX - 25, 21);
    printf("Use SETAS ou setas para trocar aba  |  ESC/ENTER para voltar");
    setColor(C_RESET);
}

/* ===================== RENDER: GAME OVER ===================== */
void renderGameOver() {
    if (!needRedrawGameOver) return;
    needRedrawGameOver = 0;

    clearScreen();
    int midX = CON_COLS / 2;

    setColor(C_GAMEOVER);
    gotoxy(midX - 10, 4);
    printf("    FIM DO LANCHE  ");

    setColor(C_SCORE);
    gotoxy(midX - 8, 6);  printf("Jogador:    %s", playerName);
    gotoxy(midX - 8, 7);  printf("Pontuacao:  %d", score);
    gotoxy(midX - 8, 8);  printf("Nivel:      %d", level);

    time_t elapsed = time(NULL) - startTime - pausedSec;
    gotoxy(midX - 8, 10); printf("Tempo:      %02d:%02d", (int)elapsed/60, (int)elapsed%60);

    const char *opts[] = { "JOGAR NOVAMENTE", "VOLTAR AO MENU" };
    for (int i = 0; i < 2; i++) {
        int oy = 13 + i * 2;
        if (i == goIndex) { setColor(C_SELECT); gotoxy(midX-13, oy); printf("%s [ %s ] %s", SYM_ZAP, opts[i], SYM_ZAP); }
        else              { setColor(C_DIM);    gotoxy(midX-9,  oy); printf("  %s  ", opts[i]); }
    }

    setColor(C_RESET);
}

/* ===================== INIT ===================== */
void initGame() {
    freeSnake();
    memset(occupancy, 0, sizeof(occupancy));
    memset(fruits, 0, sizeof(fruits));
    obstacleCount = 0;

    score = 0; level = 1; speedMs = SPEED_BASE;
    gameOver = 0; paused = 0;
    dir = DIR_RIGHT; nextDir = DIR_RIGHT;
    powerUpActive = 0; diamondActive = 0;
    lastMilestonePU = 0; lastMilestoneLVL = 0;
    pausedSec = 0;
    startTime = time(NULL);

    int sx = WIDTH / 2, sy = TOP_OFFSET + HEIGHT / 2;
    insertHead(sx, sy);

    /* Obstáculos no modo Hard */
    if (gameMode == MODE_HARD) {
        spawnObstacles();
    }

    spawnInitialFruits();

    clearScreen();
    drawBorder();
    drawPanelBox();
    if (gameMode == MODE_HARD) drawAllObstacles();
    renderSnake();
    for (int i = 0; i < FRUIT_TOTAL; i++) drawFruit(i);
    updatePanel();
    drawControls();
}

/* ===================== UPDATE ===================== */
void update() {
    if (paused || gameOver) return;

    if (autoMode) {
        if (bfs()) nextDir = bfsNextDir;
    }

    if (!((nextDir == DIR_UP    && dir == DIR_DOWN)  ||
          (nextDir == DIR_DOWN  && dir == DIR_UP)    ||
          (nextDir == DIR_LEFT  && dir == DIR_RIGHT) ||
          (nextDir == DIR_RIGHT && dir == DIR_LEFT)))
        dir = nextDir;

    int nx = snakeHead->x, ny = snakeHead->y;
    if      (dir == DIR_UP)    ny--;
    else if (dir == DIR_DOWN)  ny++;
    else if (dir == DIR_LEFT)  nx--;
    else                       nx++;

    if (nx < 0 || nx >= WIDTH ||
        ny <= TOP_OFFSET || ny >= TOP_OFFSET + HEIGHT + 1) {
        gameOver = 1; return;
    }

    /* Colisão com obstáculos (Hard) */
    if (gameMode == MODE_HARD && isObstacleAt(nx, ny - TOP_OFFSET - 1)) {
        gameOver = 1; return;
    }

    if (occupancy[ny][nx] == 1) { gameOver = 1; return; }

    int cell = occupancy[ny][nx];
    int ate  = 0;

    if (cell >= 2) {
        for (int i = 0; i < FRUIT_TOTAL; i++) {
            if (!fruits[i].active) continue;
            if (fruits[i].x == nx && arenaY(fruits[i].y) == ny) {
                int t = fruits[i].type;
                removeFruitSlot(i);

                if (t == 1) {
                    score += PTS_APPLE;
                    ate = 1;
                    spawnFruit(i, 1);
                } else if (t == 2) {
                    ate = 1;
                    powerUpActive = 1;
                    powerUpStart  = time(NULL);
                    for (int j = FRUIT_BASE; j < FRUIT_TOTAL; j++)
                        if (!fruits[j].active) spawnFruit(j, 1);
                } else if (t == 3) {
                    score += PTS_DIAMOND;
                    ate = 1;
                    diamondActive = 0;
                }
                break;
            }
        }
        if (!ate) { ate = 1; }
    }

    if (powerUpActive && difftime(time(NULL), powerUpStart) >= 10) {
        powerUpActive = 0;
        for (int i = FRUIT_BASE; i < FRUIT_TOTAL; i++) removeFruitSlot(i);
    }

    if (diamondActive && difftime(time(NULL), diamondTime) >= DIAMOND_TTL) {
        for (int i = 0; i < FRUIT_TOTAL; i++) {
            if (fruits[i].active && fruits[i].type == 3) {
                removeFruitSlot(i); break;
            }
        }
        diamondActive = 0;
    }

    if (score > 0 && score / PTS_POWERUP > lastMilestonePU) {
        lastMilestonePU = score / PTS_POWERUP;
        if (!powerUpActive) {
            for (int i = 0; i < FRUIT_TOTAL; i++)
                if (!fruits[i].active) { spawnFruit(i, 2); break; }
        }
    }

    /* Aumento de nível:
       Easy   -> nunca aumenta velocidade
       Normal -> aumenta a cada 5 níveis
       Hard   -> aumenta a cada 5 níveis + muda obstáculos */
    if (score / PTS_LEVEL > lastMilestoneLVL) {
        lastMilestoneLVL = score / PTS_LEVEL;
        level++;

        if (gameMode == MODE_NORMAL) {
            if (level % 5 == 0 && speedMs > SPEED_MIN) speedMs -= SPEED_STEP;
        } else if (gameMode == MODE_HARD) {
            if (level % 5 == 0) {
                if (speedMs > SPEED_MIN) speedMs -= SPEED_STEP;
                /* Muda obstáculos de lugar */
                spawnObstacles();
                /* Redesenha arena (limpa área de obstáculos antigos) */
                clearScreen();
                drawBorder();
                drawPanelBox();
                drawAllObstacles();
                renderSnake();
                for (int i = 0; i < FRUIT_TOTAL; i++) drawFruit(i);
                drawControls();
            }
        }
        /* Easy: sem aumento de velocidade */
    }

    if (!diamondActive && (rand() % 400) == 0) {
        for (int i = 0; i < FRUIT_TOTAL; i++) {
            if (!fruits[i].active) {
                spawnFruit(i, 3);
                diamondActive = 1;
                diamondTime   = time(NULL);
                break;
            }
        }
    }

    if (!ate && snakeTail) {
        gotoxy(arenaX(snakeTail->x), snakeTail->y);
        printf("  ");
    }

    insertHead(nx, ny);
    if (!ate) removeTail();

    renderSnake();
    updatePanel();
    drawControls();
}

/* ===================== ESTADOS DE SELECAO ===================== */
typedef enum {
    SEL_MODE=0, SEL_NAME, SEL_PLAY
} SelectionState;

SelectionState selState = SEL_MODE;

/* ===================== INPUT ===================== */
void inputMenu(State *st) {
    if (!_kbhit()) return;
    int k = _getch();
    if (k == 0 || k == 224) {
        k = _getch();
        if (k == 72) { menuIndex--; if (menuIndex < 0) menuIndex = 3; needRedrawMenu = 1; }
        if (k == 80) { menuIndex++; if (menuIndex > 3) menuIndex = 0; needRedrawMenu = 1; }
        return;
    }
    if (k=='w'||k=='W') { menuIndex--; if (menuIndex<0) menuIndex=3; needRedrawMenu=1; }
    if (k=='s'||k=='S') { menuIndex++; if (menuIndex>3) menuIndex=0; needRedrawMenu=1; }
    if (k==13) {
        if (menuIndex==0 || menuIndex==1) {
            autoMode = (menuIndex == 1) ? 1 : 0;
            modeIndex = 0;
            selState = SEL_MODE;
            /* Tela de seleção de modo */
            while (selState == SEL_MODE) {
                renderModeSelect();
                int mk = _getch();
                if (mk == 0 || mk == 224) {
                    mk = _getch();
                    if (mk == 72) { modeIndex--; if (modeIndex<0) modeIndex=2; }
                    if (mk == 80) { modeIndex++; if (modeIndex>2) modeIndex=0; }
                } else {
                    if (mk=='w'||mk=='W') { modeIndex--; if(modeIndex<0) modeIndex=2; }
                    if (mk=='s'||mk=='S') { modeIndex++; if(modeIndex>2) modeIndex=0; }
                    if (mk==13) { gameMode = (GameMode)modeIndex; selState = SEL_NAME; }
                    if (mk==27) { needRedrawMenu=1; return; }
                }
            }
            /* Pergunta nome */
            askPlayerName();
            /* Inicia jogo */
            initGame();
            *st = ST_PLAYING;
        }
        else if (menuIndex==2) { rankingTab=0; needRedrawRanking=1; *st=ST_RANKING; }
        else                   { *st=ST_EXIT; }
    }
    if (k==27) *st=ST_EXIT;
}

void inputGame(State *st) {
    if (!_kbhit()) return;
    int k = _getch();

    if (k==0||k==224) {
        k = _getch();
        if (!autoMode) {
            if (k==72) nextDir=DIR_UP;
            if (k==77) nextDir=DIR_RIGHT;
            if (k==80) nextDir=DIR_DOWN;
            if (k==75) nextDir=DIR_LEFT;
        }
        return;
    }

    if (!autoMode) {
        if (k=='w'||k=='W') nextDir=DIR_UP;
        if (k=='d'||k=='D') nextDir=DIR_RIGHT;
        if (k=='s'||k=='S') nextDir=DIR_DOWN;
        if (k=='a'||k=='A') nextDir=DIR_LEFT;
    }

    if (k=='t'||k=='T') { autoMode=!autoMode; updatePanel(); }
    if (k=='p'||k=='P') {
        if (!paused) { paused=1; pauseStart=time(NULL); }
        else { paused=0; pausedSec+=(int)difftime(time(NULL),pauseStart); }
        updatePanel();
    }
    if (k=='q'||k=='Q') { needRedrawMenu=1; *st=ST_MENU; }
}

void inputRanking(State *st) {
    if (!_kbhit()) return;
    int k = _getch();
    if (k==0||k==224) {
        k=_getch();
        if (k==75) { rankingTab--; if(rankingTab<0) rankingTab=2; needRedrawRanking=1; }
        if (k==77) { rankingTab++; if(rankingTab>2) rankingTab=0; needRedrawRanking=1; }
        return;
    }
    if (k=='a'||k=='A') { rankingTab--; if(rankingTab<0) rankingTab=2; needRedrawRanking=1; }
    if (k=='d'||k=='D') { rankingTab++; if(rankingTab>2) rankingTab=0; needRedrawRanking=1; }
    if (k==13||k==27||k=='m'||k=='M') { needRedrawMenu=1; *st=ST_MENU; }
}
void inputGameOver(State *st) {
    if (!_kbhit()) return;
    int k = _getch();
    if (k==0||k==224) {
        k=_getch();
        if(k==72){ goIndex=0; needRedrawGameOver=1; }   // ? flag aqui
        if(k==80){ goIndex=1; needRedrawGameOver=1; }   // ? e aqui
        return;
    }
    if (k=='w'||k=='W') { goIndex=0; needRedrawGameOver=1; }
    if (k=='s'||k=='S') { goIndex=1; needRedrawGameOver=1; }
    if (k==13) {
        if (goIndex==0) {
            /* Jogar novamente: manter mesmo modo e nome, só reinicia */
            initGame();
            *st=ST_PLAYING;
        } else {
            needRedrawMenu=1;
            *st=ST_MENU;
        }
    }
}

/* ===================== MAIN ===================== */
int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufSize = { (SHORT)CON_COLS, (SHORT)CON_ROWS };
    SetConsoleScreenBufferSize(hConsole, bufSize);
    SMALL_RECT wr = { 0, 0, (SHORT)(CON_COLS-1), (SHORT)(CON_ROWS-1) };
    SetConsoleWindowInfo(hConsole, TRUE, &wr);
    SetConsoleTitleA("SNAKE HUNT");

    srand((unsigned int)time(NULL));
    hideCursor();

    State state = ST_MENU;
    needRedrawMenu = 1;

    while (state != ST_EXIT) {

        if (state == ST_MENU) {
            renderMenu();
            inputMenu(&state);
            Sleep(30);
        }

        else if (state == ST_RANKING) {
            renderRanking();
            inputRanking(&state);
            Sleep(50);
        }

        else if (state == ST_PLAYING) {
            inputGame(&state);
            if (state != ST_PLAYING) continue;

            if (!paused) {
                update();

                if (gameOver) {
                    for (int f = 0; f < 4; f++) {
                        setColor(f%2==0 ? C_GAMEOVER : C_FLASH);
                        Node *cur = snakeHead;
                        while (cur) {
                            gotoxy(arenaX(cur->x), cur->y);
                            printf("%s", SYM_FIRE);
                            cur = cur->next;
                        }
                        Sleep(120);
                    }
                    if (gameMode == MODE_HARD) {
                   for (int f = 0; f < 4; f++) {
                        setColor(f%2==0 ? 12 : 14);
                         for (int i = 0; i < obstacleCount; i++) {
                        gotoxy(arenaX(obstacles[i].x), arenaY(obstacles[i].y));
                        printf("%s", f%2==0 ? SYM_FIRE : SYM_BOMB);
                        }
                        Sleep(150);
                }
                   /* Deixa os foguinhos no final */
                setColor(12);
                for (int i = 0; i < obstacleCount; i++) {
                gotoxy(arenaX(obstacles[i].x), arenaY(obstacles[i].y));
                printf("%s", SYM_FIRE);
               }
               }

                saveRanking(playerName, score);
                goIndex = 0;
                needRedrawGameOver = 1;
                state = ST_GAMEOVER;

                } else {
                    int sp = autoMode ? (speedMs > 60 ? 60 : speedMs) : speedMs;
                    Sleep(sp);
                }
            } else {
                setColor(C_PU);
                printf("  PAUSADO  ");
                Sleep(50);
            }
        }

        else if (state == ST_GAMEOVER) {
            renderGameOver();
            inputGameOver(&state);
            Sleep(50);
        }
    }

    showCursor();
    clearScreen();
    setColor(C_RESET);
    return 0;
}
