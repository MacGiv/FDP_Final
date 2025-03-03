#include "Player.h"
#include "Weapons.h"


cellStruct InitializePlayer()
{
	cellStruct playerTemp;

	playerTemp.cellChar = charPlayer;
	playerTemp.cellType = CellTypes::PLAYER;
	playerTemp.posRow = playerStartLevelPosY;
	playerTemp.posCol = playerStartLevelPosX;
	playerTemp.prevPosRow = playerStartLevelPosY;
	playerTemp.prevPosCol = playerStartLevelPosX;

	return playerTemp;
}


void ProcessPlayerMovement(bool& playerHasMoved, cellStruct& playerStruct, cellStruct myMatrix[mapSizeRows][mapSizeCols], char inputChar)
{
	//Get next pos of the player
	switch (inputChar)
	{
	case 'w':
	case 'W':
		switch (myMatrix[playerStruct.posRow - 1][playerStruct.posCol].cellType)
		{
		case CellTypes::WALKABLE:
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.posRow -= 1;
			playerHasMoved = true;
			break;
		case CellTypes::WALL:
			playerHasMoved = false;
			break;
		case CellTypes::EXIT:
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.posRow -= 1;
			playerHasMoved = true;
			break;
		default:
			playerHasMoved = false;
			break;
		}
		break;
	case 's':
	case 'S':
		switch (myMatrix[playerStruct.posRow + 1][playerStruct.posCol].cellType)
		{
		case CellTypes::WALKABLE:
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.posRow += 1;
			playerHasMoved = true;
			break;
		case CellTypes::WALL:
			playerHasMoved = false;
			break;
		case CellTypes::EXIT:
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.posRow += 1;
			playerHasMoved = true;
			break;
		default:
			playerHasMoved = false;
			break;
		}
		break;
	case 'a':
	case 'A':
		switch (myMatrix[playerStruct.posRow][playerStruct.posCol - 1].cellType)
		{
		case CellTypes::WALKABLE:
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.posCol -= 1;
			playerHasMoved = true;
			break;
		case CellTypes::WALL:
			playerHasMoved = false;
			break;
		case CellTypes::EXIT:
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.posCol -= 1;
			playerHasMoved = true;
			break;
		default:
			playerHasMoved = false;
			break;
		}
		break;
		break;
	case 'd':
	case 'D':
		switch (myMatrix[playerStruct.posRow][playerStruct.posCol + 1].cellType)
		{
		case CellTypes::WALKABLE:
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.posCol += 1;
			playerHasMoved = true;
			break;
		case CellTypes::WALL:
			playerHasMoved = false;
			break;
		case CellTypes::EXIT:
			playerStruct.prevPosCol = playerStruct.posCol;
			playerStruct.prevPosRow = playerStruct.posRow;
			playerStruct.posCol += 1;
			playerHasMoved = true;
			break;
		default:
			playerHasMoved = false;
			break;
		}
		break;
	default:
		playerHasMoved = false;
		break;
	}
}

AttackDirections GetAttackDirection(int inputChar)
{
	switch (inputChar)
	{
	case 'i':
	case 'I':
		//attack north
		return AttackDirections::NORTH;
		break;
	case 'k':
	case 'K':
		//Attack south
		return AttackDirections::SOUTH;
		break;
	case 'j':
	case 'J':
		//Attack west
		return AttackDirections::WEST;
		break;
	case 'l':
	case 'L':
		//Attack east
		return AttackDirections::EAST;
		break;
	default:
		return AttackDirections::WEST;
		break;
	}
}

void PlayerAttackDagger(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, attackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell)
{
	GetAttackPositionsDagger(attackPos, attackDirection, playerCell);
	for (int i = 0; i < daggerAttacksPosAmount; i++)
	{
		IsAttackPossible(map, attackPos[i]);
		if (attackPos[i].attackPossible)
		{
			map[attackPos[i].row][attackPos[i].col].cellType = CellTypes::PLAYER_ATTACK;
		}
	}
}


//Returns true if inputChar is W, A, S, or D
bool IsMovementInput(char inputChar)
{
	switch (inputChar)
	{
	case 'w':
	case 'W':
		return true;
		break;
	case 's':
	case 'S':
		return true;
		break;
	case 'a':
	case 'A':
		return true;
		break;
		break;
	case 'd':
	case 'D':
		return true;
		break;
	default:
		return false;
		break;
	}
}

//Returns true if inputChar is I, J, K, or L
bool IsAttackInput(char inputChar)
{
	switch (inputChar)
	{
	case 'i':
	case 'I':
		return true;
		break;
	case 'k':
	case 'K':
		return true;
		break;
	case 'j':
	case 'J':
		return true;
		break;
	case 'l':
	case 'L':
		return true;
		break;
	default:
		return false;
		break;
	}
}

//Returns true if the attempted attack position is in range and an attackable cell, otherwise returns false
void IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], attackPosition& attackPos)
{
	bool inRange = false;
	bool canAttackCellType = false;
	bool isInRowsRange = false;
	bool isInColsRange = false;

	isInRowsRange = (attackPos.row <= (mapSizeRows - 1) && attackPos.row >= (minPosMap + 1));
	isInColsRange = (attackPos.col <= (mapSizeCols - 1) && attackPos.col >= (minPosMap + 1));
	
	if (isInRowsRange && isInColsRange)
		inRange = true;
	else
		inRange = false;

	if (inRange)
	{
		switch (map[attackPos.row][attackPos.col].cellType)
		{
		case CellTypes::DEFAULT:
			attackPos.attackPossible = false;
			break;
		case CellTypes::WALKABLE:
			attackPos.attackPossible = true;
			break;
		case CellTypes::WALL:
			attackPos.attackPossible = false;
			break;
		case CellTypes::ENEMY:
			attackPos.attackPossible = true;
			break;
		case CellTypes::EXIT:
			attackPos.attackPossible = false;
			break;
		default:
			attackPos.attackPossible = false;
			break;
		}
	}
	else
	{
		attackPos.attackPossible = false;
	}
}