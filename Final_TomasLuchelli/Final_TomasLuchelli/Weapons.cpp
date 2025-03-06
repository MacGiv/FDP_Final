#include "Weapons.h"

// These functions get the positions of the attacks depending on the weapon used.

void GetAttackPositionsDagger(attackPosition attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
	attackPosition auxPos;
	int i = 0;
	
	for (i = 0; i < daggerAttacksPosAmount; i++)
	{
		switch (direction)
		{
		case AttackDirections::NORTH:
			auxPos.row = playerStruct.posRow - 1;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::SOUTH:
			auxPos.row = playerStruct.posRow + 1;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::EAST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol + 1;
			break;
		case AttackDirections::WEST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol - 1;
			break;
		default:
			break;
		}

		attackPos[i] = auxPos;
	}
	
}

void GetAttackPositionsSword(attackPosition attackPos[swordAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
	attackPosition auxPos;
	int i = 0;

	for (i = 0; i < swordAttacksPosAmount; i++)
	{
		switch (direction)
		{
		case AttackDirections::NORTH:
			auxPos.row = playerStruct.posRow - 1 - i;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::SOUTH:
			auxPos.row = playerStruct.posRow + 1 + i;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::EAST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol + 1 + i;
			break;
		case AttackDirections::WEST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol - 1 - i; 
			break;
		default:
			break;
		}

		attackPos[i] = auxPos;
	}
}

void GetAttackPositionsSword(attackPosition attackPos[swordAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
	attackPosition auxPos;
	int i = 0;

	for (i = 0; i < swordAttacksPosAmount; i++)
	{
		switch (direction)
		{
		case AttackDirections::NORTH:
			auxPos.row = playerStruct.posRow - 1 - i;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::SOUTH:
			auxPos.row = playerStruct.posRow + 1 + i;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::EAST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol + 1 + i;
			break;
		case AttackDirections::WEST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol - 1 - i;
			break;
		default:
			break;
		}

		attackPos[i] = auxPos;
	}
}