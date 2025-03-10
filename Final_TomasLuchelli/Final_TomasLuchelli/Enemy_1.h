#pragma once

#include "Windows.h"

#include "GameData.h"
#include "Utilities.h"

enum class EnemyDirection { UP, DOWN, LEFT, RIGHT };
enum class EnemyAttackType { CROSS, DIAGONAL, SURROUND };

int const maxEnemyAttacksP1 = 16; 
int const maxEnemyAttacksP2 = 24; 
int const maxEnemyAttacksP3 = 32; 

struct EnemyCell
{
	cellStruct cell;
	int hp = 100;
	int id = 0;
	bool hasMoved = false;
	bool isAlive = false;
	bool isColliding = false;
	EnemyDirection direction = EnemyDirection::DOWN;
	EnemyAttackType attackType = EnemyAttackType::SURROUND;
};

struct enemyAttackPosition
{
	int row = 0;
	int col = 0;
	bool isActive = false;
};

void InitializeEnemy(EnemyCell& enemy, int posRow, int posCol, int id);

void MoveEnemy(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveUp(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveDown(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveRight(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveLeft(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyAttack(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols], EnemyAttackType, enemyAttackPosition enemyAttacks[maxEnemyAttacksP1]);

void DrawEnemy(EnemyCell enemyToDraw, cellStruct map[mapSizeRows][mapSizeCols], HANDLE& hConsole);