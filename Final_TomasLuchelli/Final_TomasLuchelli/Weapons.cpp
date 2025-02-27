#include "Weapons.h"

//These functions return the positions of the diferent attack types.

void GetAttackPositions(attackPosition attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
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

/*
void GetAttackPositions(int attackPos[swordAttacksPosAmount], AttackDirections direction)
{
	AttackPosition auxPos;

}

void GetAttackPositions(int attackPos[axeAttacksPosAmount], AttackDirections direction)
{
	AttackPosition auxPos;

}

void GetAttackPositions(int attackPos[poleAttacksPosAmount], AttackDirections direction)
{
	AttackPosition auxPos;

}

void GetAttackPositions(int attackPos[poleAxeAttacksPosAmount], AttackDirections direction)
{
	AttackPosition auxPos;

}
*/