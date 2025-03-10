#pragma once
#include "GameData.h"
#include "Utilities.h"
#include "Weapons.h"

// Player data
int extern playerHp;
bool extern hitReceived;

cellStruct InitializePlayer();

AttackDirections GetAttackDirection(int inputChar);


// Returns true if inputChar is W, A, S, or D
bool IsMovementInput(char inputChar);

// Returns true if inputChar is I, J, K, or L
bool IsAttackInput(char inputChar);

// Returns true if inputChar is 1, 2, 3, 4 ,5
bool IsWeaponInput(char inputChar);

void ProcessPlayerMovement(bool& playerHasMoved, cellStruct& playerStruct, cellStruct myMatrix[mapSizeRows][mapSizeCols], char inputChar);

// Player Attacks
void PlayerAttackDagger(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell);
void PlayerAttackSword(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[swordAttacksPosAmount], cellStruct playerCell);
void PlayerAttackAxe(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[axeAttacksPosAmount], cellStruct playerCell);
void PlayerAttackPole(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[poleAttacksPosAmount], cellStruct playerCell);
void PlayerAttackPoleaxe(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[poleaxeAttacksPosAmount], cellStruct playerCell);

// Checks if the attempted attack is in range and puts the bool attackPossible in attackPosition's struct in true/false
void IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], playerAttackPosition& attackPos);