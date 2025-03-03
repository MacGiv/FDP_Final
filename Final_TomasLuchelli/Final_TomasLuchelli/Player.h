#pragma once
#include "GameData.h"
#include "Utilities.h"

enum class Weapons {DAGGER = 1, SWORD, AXE, POLE, POLEAXE};

Weapons extern playerCurrentWeapon;

cellStruct InitializePlayer();

AttackDirections GetAttackDirection(int inputChar);

//Returns true if inputChar is W, A, S, or D
bool IsMovementInput(char inputChar);

//Returns true if inputChar is I, J, K, or L
bool IsAttackInput(char inputChar);

void ProcessPlayerMovement(bool& playerHasMoved, cellStruct& playerStruct, cellStruct myMatrix[mapSizeRows][mapSizeCols], char inputChar);

//Player Dagger Attack
void PlayerAttackDagger(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, attackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell);

// Checks if the attempted attack position is in range, otherwise returns false
void IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], attackPosition& attackPos);