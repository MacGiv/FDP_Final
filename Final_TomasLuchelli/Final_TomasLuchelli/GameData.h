#pragma once

//Walls characters
const char charWallTL = 201; // Top Left
const char charWallTR = 187; // Top Right
const char charWallBL = 200; // Bottom Left
const char charWallBR = 188; // Bottom Right
const char charWallH = 205;	 // Horizontal
const char charWallV = 186;	 // Vertical

//Colors
const int colorWall = 7;		 // White
const int colorPlayer = 14;		 // Yellow
const int colorWalkable = 8;	 // Gray
const int colorExit = 11;		 // Cyan
const int colorEnemy = 12;		 // Red
const int colorPlayerAttack = 9; // Blue

//Special Characters
const char charPlayer = 233;
const char charTrap = 232;
const char charEnemy = 134;
const char charEnemyBoss = 143;
const char charExit = 'E';
const char charEmpty = 249;
const char charEscapeKey = 27;
const char charPlayerAttack= 120;
const char charEnemyAttack= 120;



//Map and position related variables
const int minPosMap = 0;
const int mapSizeRows = 9;
const int mapSizeCols = 40;
const int playerStartLevelPosX = 6;
const int playerStartLevelPosY = 4;
const int mapStartPosX = 4;
const int mapStartPosY = 4;
const int exitLevelPosX =  mapSizeCols -2;
const int exitLevelPosY = 4;
const int objetiveInfoPosX = 0;
const int objetiveInfoPosY = 1;
const int levelNumberInfoPosX = 0;
const int levelNumberInfoPosY = 0;
const int timeInfoPosX = 0;
const int timeInfoPosY = mapStartPosY + mapSizeRows + 1;
const int fpsInfoPosX = 0;
const int fpsInfoPosY = timeInfoPosY + 1;
const int endLevelMsgPosX = 0;
const int endLevelMsgPosY = fpsInfoPosY + 1;

//Amount of cells to attack depending on the weapon type:
//Dagger attacks 1 cell.
//Sword attacks 2 cells.
//Axe attacks 3 cells.
//Pole attacks 1 cell.
//Poleaxe attacks 3 cells.
int const daggerAttacksPosAmount = 1;
int const swordAttacksPosAmount = 2;
int const axeAttacksPosAmount = 3;
int const poleAttacksPosAmount = 1;
int const poleAxeAttacksPosAmount = 3;

enum class LevelTypes { TUTORIAL, TRAPS, ENEMIES}; //TODO Used?

enum class CellTypes {DEFAULT, WALKABLE, WALL, PLAYER, ENEMY, EXIT, PLAYER_ATTACK, ENEMY_ATTACK };

enum class AttackDirections { NORTH, SOUTH, EAST, WEST };

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

//Externs
extern int playerLevelProgress;
extern double startTime;
extern double currentTime;
extern double lastTime;
extern int fps;
extern int frameCount;
