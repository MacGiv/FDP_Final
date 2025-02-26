#pragma once
#include "GameData.h"



struct AttackPosition
{
	int row = 0;
	int col = 0;
}; 


//These functions return the positions of the diferent attack types.
//The array 'attackPos' will hold the positions of the attacks in order following order:
//Pos 0: row -> Pos 1: column -> Pos 2: row -> Pos 4: column -> etc 
void GetAttackPositions(int attackPos[daggerAttacksPosAmount*2], AttackDirections direction, cellStruct playerStruct);

//void GetAttackPositions(int attack[swordAttacksPosAmount], AttackDirections direction);
//void GetAttackPositions(int attack[axeAttacksPosAmount], AttackDirections direction);
//void GetAttackPositions(int attack[poleAttacksPosAmount], AttackDirections direction);
//void GetAttackPositions(int attack[poleAxeAttacksPosAmount], AttackDirections direction);