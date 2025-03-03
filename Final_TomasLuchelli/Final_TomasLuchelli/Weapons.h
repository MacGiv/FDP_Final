#pragma once
#include "GameData.h"






// These functions get the positions of the attacks depending on the weapon used. The positions are stored in the attackPos array.

void GetAttackPositionsDagger(attackPosition attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);

void GetAttackPositionsSword(attackPosition attackPos[swordAttacksPosAmount], AttackDirections direction, cellStruct playerStruct);
