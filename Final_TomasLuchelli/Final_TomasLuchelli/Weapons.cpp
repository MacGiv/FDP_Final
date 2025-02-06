#include "Weapons.h"

//These functions return the positions of the diferent attack types.
//The array 'attackPos' will hold the positions of the attacks in order following order:
//Pos 0: row -> Pos 1: column -> Pos 2: row -> Pos 4: column -> etc 

void GetAttackPositions(int attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
	AttackPosition auxPos;
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

		attackPos[i] = auxPos.row;
		i++;
		attackPos[i] = auxPos.col;
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