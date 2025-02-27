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

//Player Attack
void PlayerDaggerAttack(cellStruct map[mapSizeRows][mapSizeCols], Weapons playerWeapon, AttackDirections attackDirection, attackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell);

//Returns true if the attempted attack position is in range, otherwise returns false
bool IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], int attackPosRow, int attackPosCol);