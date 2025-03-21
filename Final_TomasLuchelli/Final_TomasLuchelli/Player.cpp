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

// Returns true if inputChar is 1, 2, 3, 4 ,5
bool IsWeaponInput(char inputChar)
{
	if (static_cast<int>(inputChar) > 48 && 
		static_cast<int>(inputChar) < 54)
	{
		return true;
	}
	else
	{
		return false;
	}
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

// Get the positions of the Dagger's attack and check if they are valid. If the attack is valid, set the map cells to the PLAYER_ATTACK type and populate the Attack Position's array.
void PlayerAttackDagger(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell)
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

// Get the positions of the Sword's attack and check if they are valid. If the attack is valid, set the map cells to the PLAYER_ATTACK type and populate the Attack Position's array.
void PlayerAttackSword(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[swordAttacksPosAmount], cellStruct playerCell)
{
	GetAttackPositionsSword(attackPos, attackDirection, playerCell);
	for (int i = 0; i < swordAttacksPosAmount; i++)
	{
		IsAttackPossible(map, attackPos[i]);
		if (attackPos[i].attackPossible)
		{
			map[attackPos[i].row][attackPos[i].col].cellType = CellTypes::PLAYER_ATTACK;
		}
	}
}

void PlayerAttackAxe(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[axeAttacksPosAmount], cellStruct playerCell)
{
	GetAttackPositionsAxe(attackPos, attackDirection, playerCell);
	for (int i = 0; i < axeAttacksPosAmount; i++)
	{
		IsAttackPossible(map, attackPos[i]);
		if (attackPos[i].attackPossible)
		{
			map[attackPos[i].row][attackPos[i].col].cellType = CellTypes::PLAYER_ATTACK;
		}
	}
}

void PlayerAttackPole(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[poleAttacksPosAmount], cellStruct playerCell)
{
	GetAttackPositionsPole(attackPos, attackDirection, playerCell);
	for (int i = 0; i < poleAttacksPosAmount; i++)
	{
		IsAttackPossible(map, attackPos[i]);
		if (attackPos[i].attackPossible)
		{
			map[attackPos[i].row][attackPos[i].col].cellType = CellTypes::PLAYER_ATTACK;
		}
	}
}

void PlayerAttackPoleaxe(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[poleaxeAttacksPosAmount], cellStruct playerCell)
{
	GetAttackPositionsPoleaxe(attackPos, attackDirection, playerCell);
	for (int i = 0; i < poleaxeAttacksPosAmount; i++)
	{
		IsAttackPossible(map, attackPos[i]);
		if (attackPos[i].attackPossible)
		{
			map[attackPos[i].row][attackPos[i].col].cellType = CellTypes::PLAYER_ATTACK;
		}
	}
}

// Checks if the attempted attack is in range and puts the bool attackPossible in attackPosition's struct in true/false
void IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], playerAttackPosition& attackPos)
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
