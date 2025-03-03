#pragma once
#include "GameData.h"






//These functions return the positions of the diferent attack types.
//The array 'attackPos' will hold the positions of the attacks in order following order:
//Pos 0: row -> Pos 1: column -> Pos 2: row -> Pos 4: column -> etc 
void GetAttackPositionsDagger(attackPosition attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);

//void GetAttackPositions(int attack[swordAttacksPosAmount], AttackDirections direction);
//void GetAttackPositions(int attack[axeAttacksPosAmount], AttackDirections direction);
//void GetAttackPositions(int attack[poleAttacksPosAmount], AttackDirections direction);
//void GetAttackPositions(int attack[poleAxeAttacksPosAmount], AttackDirections direction);