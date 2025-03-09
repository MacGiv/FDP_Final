#pragma once

#include "Windows.h"

#include "GameData.h"
#include "Utilities.h"

enum class EnemyDirection { UP, DOWN, LEFT, RIGHT };
enum class EnemyAttackType { CROSS, DIAGONAL, SURROUND };

struct EnemyCell
{
	cellStruct cell;
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

void InitializeEnemy(EnemyCell& enemy, int posRow, int posCol);

void MoveEnemy(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveUp(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveDown(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveRight(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void EnemyMoveLeft(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols]);

void DrawEnemy(EnemyCell enemyToDraw, cellStruct map[mapSizeRows][mapSizeCols], HANDLE& hConsole);