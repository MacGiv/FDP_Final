#pragma once
#include "GameData.h"


enum class Weapons { DAGGER = 1, SWORD, AXE, POLE, POLEAXE };

void GetAttackPositionsDagger(playerAttackPosition attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);

void GetAttackPositionsSword(playerAttackPosition attackPos[swordAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);

void GetAttackPositionsAxe(playerAttackPosition attackPos[axeAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);

void GetAttackPositionsPole(playerAttackPosition attackPos[poleAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);

void GetAttackPositionsPoleaxe(playerAttackPosition attackPos[poleaxeAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);