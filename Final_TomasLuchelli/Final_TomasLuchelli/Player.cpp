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
		break;
	}
}

void PlayerAttack(cellStruct map[mapSizeRows][mapSizeCols], Weapons playerWeapon, AttackDirections attackDirection, attackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell)
{
	switch (playerWeapon)
	{
	case Weapons::DAGGER:
		GetAttackPositions(attackPos, attackDirection, playerCell);
		if (IsAttackPossible(map, attackPos[0].row, attackPos[0].col))
		{
			map[attackPos[0].row][attackPos[0].col].cellType = CellTypes::PLAYER_ATTACK;
		}
		
		break;
	case Weapons::SWORD:

		break;
	case Weapons::AXE:

		break;
	case Weapons::POLE:

		break;
	case Weapons::POLEAXE:

		break;
	default:
		break;
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
bool IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], int attackPosRow, int attackPosCol)
{
	bool inRange = false;
	bool canAttackCellType = false;

	bool isInRowsRange = (attackPosRow <= mapSizeRows && attackPosRow >= minPosMap);
	bool isInColsRange = (attackPosCol <= mapSizeCols && attackPosCol >= minPosMap);
	
	if (isInRowsRange && isInColsRange)
		inRange = true;
	else
		inRange = false;

	if (inRange)
	{
		switch (map[attackPosRow][attackPosCol].cellType)
		{
		case CellTypes::DEFAULT:
			return false;
			break;
		case CellTypes::WALKABLE:
			return true;
			break;
		case CellTypes::WALL:
			return false;
			break;
		case CellTypes::ENEMY:
			return true;
			break;
		case CellTypes::EXIT:
			return false;
			break;
		default:
			return false;
			break;
		}
	}
	else
	{
		return false;
	}
}