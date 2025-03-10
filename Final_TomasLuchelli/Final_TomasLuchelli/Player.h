#pragma once
#include "GameData.h"
#include "Utilities.h"

enum class Weapons {DAGGER = 1, SWORD, AXE, POLE, POLEAXE};

// Player data
int extern playerHp;
bool extern hitReceived;
Weapons extern playerCurrentWeapon;

cellStruct InitializePlayer();

AttackDirections GetAttackDirection(int inputChar);

// Returns true if inputChar is W, A, S, or D
bool IsMovementInput(char inputChar);

// Returns true if inputChar is I, J, K, or L
bool IsAttackInput(char inputChar);

void ProcessPlayerMovement(bool& playerHasMoved, cellStruct& playerStruct, cellStruct myMatrix[mapSizeRows][mapSizeCols], char inputChar);

// Player Attacks
void PlayerAttackDagger(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[daggerAttacksPosAmount], cellStruct playerCell);

void PlayerAttackSword(cellStruct map[mapSizeRows][mapSizeCols], AttackDirections attackDirection, playerAttackPosition attackPos[swordAttacksPosAmount], cellStruct playerCell);

// Checks if the attempted attack is in range and puts the bool attackPossible in attackPosition's struct in true/false
void IsAttackPossible(cellStruct map[mapSizeRows][mapSizeCols], playerAttackPosition& attackPos);