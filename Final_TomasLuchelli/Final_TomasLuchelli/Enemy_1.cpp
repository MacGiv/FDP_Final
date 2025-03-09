#include "Enemy_1.h"

void MarkAttackCell(cellStruct map[mapSizeRows][mapSizeCols], int row, int col);

void InitializeEnemy(EnemyCell& enemy, int posRow, int posCol)
{
	cellStruct enemyTemp;

	enemyTemp.cellChar = charEnemy;
	enemyTemp.cellType = CellTypes::ENEMY;
	enemyTemp.posRow = posRow;
	enemyTemp.posCol = posCol;
	enemyTemp.prevPosRow = enemyTemp.posRow;
	enemyTemp.prevPosCol = enemyTemp.posCol;

	enemy.cell = enemyTemp;
	enemy.hasMoved = false;
	enemy.isAlive = true;
	enemy.isColliding = false;
	enemy.direction = EnemyDirection::UP;
}

void MoveEnemy(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols])
{
	switch (enemy.direction)
	{
	case EnemyDirection::UP:
		EnemyMoveUp(enemy, map);
		break;
	case EnemyDirection::DOWN:
		EnemyMoveDown(enemy, map);
		break;
	case EnemyDirection::LEFT:
		EnemyMoveLeft(enemy, map);
		break;
	case EnemyDirection::RIGHT:
		EnemyMoveRight(enemy, map);
		break;
	}

	// If collision, change direction
	if (enemy.isColliding)
	{
		enemy.isColliding = false;

		switch (enemy.direction)
		{
		case EnemyDirection::UP:
			enemy.direction = EnemyDirection::DOWN;
			break;
		case EnemyDirection::DOWN:
			enemy.direction = EnemyDirection::UP;
			break;
		case EnemyDirection::LEFT:
			enemy.direction = EnemyDirection::RIGHT;
			break;
		case EnemyDirection::RIGHT:
			enemy.direction = EnemyDirection::LEFT;
			break;
		}
	}
}

void EnemyMoveUp(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols])
{
	switch (map[enemy.cell.posRow - 1][enemy.cell.posCol].cellType)
	{
	case CellTypes::WALKABLE:
		enemy.cell.prevPosRow = enemy.cell.posRow;
		enemy.cell.prevPosCol = enemy.cell.posCol;
		enemy.cell.posRow -= 1;
		enemy.hasMoved = true;
		map[enemy.cell.posRow][enemy.cell.posCol].cellType = CellTypes::ENEMY;
		break;
	case CellTypes::WALL:
		enemy.hasMoved = false;
		enemy.isColliding = true;
		break;
	default:
		enemy.hasMoved = false;
		break;
	}
}

void EnemyMoveDown(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols])
{
	switch (map[enemy.cell.posRow + 1][enemy.cell.posCol].cellType)
	{
	case CellTypes::WALKABLE:
		enemy.cell.prevPosRow = enemy.cell.posRow;
		enemy.cell.prevPosCol = enemy.cell.posCol;
		enemy.cell.posRow += 1;
		enemy.hasMoved = true;
		map[enemy.cell.posRow][enemy.cell.posCol].cellType = CellTypes::ENEMY;
		break;
	case CellTypes::WALL:
		enemy.hasMoved = false;
		enemy.isColliding = true;
		break;
	default:
		enemy.hasMoved = false;
		break;
	}
}

void EnemyMoveRight(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols])
{
	switch (map[enemy.cell.posRow][enemy.cell.posCol + 1].cellType)
	{
	case CellTypes::WALKABLE:
		enemy.cell.prevPosRow = enemy.cell.posRow;
		enemy.cell.prevPosCol = enemy.cell.posCol;
		enemy.cell.posCol += 1;
		enemy.hasMoved = true;
		map[enemy.cell.posRow][enemy.cell.posCol].cellType = CellTypes::ENEMY;
		break;
	case CellTypes::WALL:
		enemy.hasMoved = false;
		enemy.isColliding = true;
		break;
	default:
		enemy.hasMoved = false;
		break;
	}
}

void EnemyMoveLeft(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols])
{
	switch (map[enemy.cell.posRow][enemy.cell.posCol - 1].cellType)
	{
	case CellTypes::WALKABLE:
		enemy.cell.prevPosRow = enemy.cell.posRow;
		enemy.cell.prevPosCol = enemy.cell.posCol;
		enemy.cell.posCol -= 1;
		enemy.hasMoved = true;
		map[enemy.cell.posRow][enemy.cell.posCol].cellType = CellTypes::ENEMY;
		break;
	case CellTypes::WALL:
		enemy.hasMoved = false;
		enemy.isColliding = true;
		break;
	default:
		enemy.hasMoved = false;
		break;
	}
}

void EnemyAttack(EnemyCell& enemy, cellStruct map[mapSizeRows][mapSizeCols], EnemyAttackType attackType)
{
	int row = enemy.cell.posRow;
	int col = enemy.cell.posCol;

	switch (attackType)
	{
	case EnemyAttackType::CROSS: // "+"
		MarkAttackCell(map, row - 1, col); // Up
		MarkAttackCell(map, row + 1, col); // Down
		MarkAttackCell(map, row, col - 1); // Left
		MarkAttackCell(map, row, col + 1); // Right
		break;

	case EnemyAttackType::DIAGONAL: // "x"
		MarkAttackCell(map, row - 1, col - 1); // Top Left 
		MarkAttackCell(map, row - 1, col + 1); // Top Right
		MarkAttackCell(map, row + 1, col - 1); // Bot Left 
		MarkAttackCell(map, row + 1, col + 1); // Bot Right
		break;

	case EnemyAttackType::SURROUND:
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i != 0 || j != 0)
				{
					MarkAttackCell(map, row + i, col + j);
				}
			}
		}
		break;
	}
}

void MarkAttackCell(cellStruct map[mapSizeRows][mapSizeCols], int row, int col)
{
	if (map[row][col].cellType == CellTypes::WALKABLE || map[row][col].cellType == CellTypes::PLAYER)
	{
		map[row][col].cellType = CellTypes::ENEMY_ATTACK;
		map[row][col].cellChar = charEnemyAttack;
	}
}

void DrawEnemy(EnemyCell enemyToDraw, cellStruct map[mapSizeRows][mapSizeCols], HANDLE& hConsole)
{
	Gotoxy(mapStartPosX + enemyToDraw.cell.posCol, mapStartPosY + enemyToDraw.cell.posRow);
	SetConsoleTextAttribute(hConsole, colorEnemy);
	std::cout << charEnemy;
	SetConsoleTextAttribute(hConsole, colorWall);
}