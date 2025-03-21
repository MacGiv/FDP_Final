#pragma once

// Walls characters
const char charWallTL = 201; // Top Left
const char charWallTR = 187; // Top Right
const char charWallBL = 200; // Bottom Left
const char charWallBR = 188; // Bottom Right
const char charWallH = 205;	 // Horizontal
const char charWallV = 186;	 // Vertical

// Colors
const int colorWall = 7;		 // White
const int colorPlayer = 14;		 // Yellow
const int colorWalkable = 8;	 // Gray
const int colorExit = 11;		 // Cyan
const int colorEnemy = 12;		 // Red
const int colorPlayerAttack = 9; // Blue
const int colorEnemyAttack = 13; // Magenta

// Special Characters
const char charPlayer = 233;
const char charTrap = 232;
const char charEnemy = 134;
const char charEnemyBoss = 143;
const char charExit = 'E';
const char charEmpty = 249;
const char charEscapeKey = 27;
const char charPlayerAttack= 120;
const char charEnemyAttack= 120;

// Map and position related variables
const int minPosMap = 0;
const int mapSizeRows = 13;
const int mapSizeCols = 60;
const int mapCenterPosY = (mapSizeRows - 1) / 2;
const int mapOneFifthPosX = (mapSizeCols - 1) / 5;
const int playerStartLevelPosX = 6;
const int playerStartLevelPosY = mapCenterPosY;
const int mapStartPosX = 4;
const int mapStartPosY = 4;
const int exitLevelPosX =  mapSizeCols -2;
const int exitLevelPosY = mapCenterPosY;
const int infoOnePosX = 0;
const int infoOnePosY = 1;
const int infoTwoPosX = 0;
const int infoTwoPosY = 2;
const int infoThreePosX = 0;
const int infoThreePosY = 3;
const int levelNumberInfoPosX = 0;
const int levelNumberInfoPosY = 0;
const int timeInfoPosX = 0;
const int timeInfoPosY = mapStartPosY + mapSizeRows + 1;
const int fpsInfoPosX = 0;
const int fpsInfoPosY = timeInfoPosY + 1;
const int playerHpPosX = 0;
const int playerHpPosY = fpsInfoPosY + 1;
const int endLevelMsgPosX = 0;
const int endLevelMsgPosY = playerHpPosY + 1;

// Amount of cells to attack depending on the weapon  type:
// Dagger attacks 1 cell.
// Sword attacks 2 cells.
// Axe attacks 3 cells.
// Pole attacks 3 cell.
// Poleaxe attacks 5 cells.
int const daggerAttacksPosAmount = 1;
int const swordAttacksPosAmount = 2;
int const axeAttacksPosAmount = 4;
int const poleAttacksPosAmount = 3;
int const poleaxeAttacksPosAmount = 5;


enum class CellTypes {DEFAULT, WALKABLE, WALL, PLAYER, ENEMY, EXIT, PLAYER_ATTACK, ENEMY_ATTACK };

enum class AttackDirections { NORTH, SOUTH, EAST, WEST };

enum class Weapons { DAGGER = 1, SWORD, AXE, POLE, POLEAXE };

struct cellStruct
{
	CellTypes cellType = CellTypes::DEFAULT;
	char cellChar = 'D';
	char cellMainChar = charEmpty;
	int posRow = 0;
	int posCol = 0;
	int prevPosRow = 0;
	int prevPosCol = 0;
};

struct playerAttackPosition
{
	int row = 0;
	int col = 0;
	bool attackPossible = false;
};

extern playerAttackPosition attackPositionsArrayDagger[daggerAttacksPosAmount];
extern playerAttackPosition attackPositionsArraySword[swordAttacksPosAmount];
extern playerAttackPosition attackPositionsArrayAxe[axeAttacksPosAmount];
extern playerAttackPosition attackPositionsArrayPole[poleAttacksPosAmount];
extern playerAttackPosition attackPositionsArrayPoleaxe[poleaxeAttacksPosAmount];

// Externs
extern int playerPhase;
extern double startTime;
extern double currentTime;
extern double lastTime;
extern int fps;
extern int frameCount;
extern Weapons playerCurrentWeapon;