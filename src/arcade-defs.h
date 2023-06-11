//Game defines
enum GAMEMODE {ARCADE, SNAKE, PATH, MAZES, PONG, TETRIS, SPACE, 
FROGGER, CONWAY, LANGSTON, FLAPPY, CONNECT4, CHECKERS, MINES, 
CHESS, BATTLE1, BATTLE2, SUDOKU, BREAKOUT, COLLAPSE, TRON, 
SETTINGS, SETTINGS2, SETTINGS3, SETUP};

//Color defines
enum COLOR {BLACK = -1, WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN};

//Number image defines
enum NUMBER {ZERO = 7, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

//Chess defines
enum CHESSPIECE {P1PAWN = 17, P1ROOK, P1BISHOP, P1KNIGHT, P1QUEEN, P1KING, 
P2PAWN, P2ROOK, P2BISHOP, P2KNIGHT, P2QUEEN, P2KING};

//Menu image defines
enum MENUIMG {settings_img = 57, fullscreen_img = 30, confirm_img, cancel_img, 
width_img, height_img, cellsize_img, arrow_img, arrow_flip, TITLE};

enum COUNTS {TEXTURES = 60};

//Menu defines
enum MENUSIZE {ROWS = 24, //Main menu two-sided rows
SROWS = 10}; //Settings menu rows

//Battle1
enum Battle1 {EMPTY = -1, CLEAR, SHIP};

//Battle2
enum Battle2 {P1SHIP = 1, P1SHIPATTACKED, P2SHIP, P2SHIPATTACKED = 5};

//Breakout
enum Breakout {PADDLE, BALL, BLOCK};

//checkers
enum Checkers {P1CPAWN, P2CPAWN, P1CKING, P2CKING, P1MOVE, P2MOVE};

//chess
enum Chess {PMOVE = 2};

//connect4
enum Connect4 {PLAYER1, PLAYER2};

//conway
enum Conway {LIFE};

//flappy
enum Flappy {OBSTACLE, BIRD, HOLESIZE = 5};

//frogger
enum Frogger {LOG, CAR, FROG, WATER};

//langston
enum Langston {ANT = 1};

//mazes
enum Mazes {WALL, PLAYER, START, END};

//mines
enum Mines {MINE = 1, FLAG = 4};

//path
enum Path {CLOSED = 1, OPEN = 4, FINAL};

//pong

//setup
enum Setup {FILLED};

//snake
enum Snake {FOOD};

//space
enum Space {MOVED = -2, ENEMY = 0, EBULLET = 2, PBULLET, HOUSE, LAG = 8};

//Tetris
enum Tetris {UP, RIGHT, DOWN, LEFT};

//Mine Collapse
enum Collapse {EXTIMER = -10, //Explosion Timer
MWALL = 0, BOMB, MINER, EXIT, EXPLOSION, BWALL, CARTUP, CARTDOWN, CARTLEFT, CARTRIGHT, 
BTIMER = 10}; //Bomb Timer

//Snaketron
enum Tron {TRON1 = 1, TRON2};