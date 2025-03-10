#include "GameData.h"

int playerPhase = 1;
double startTime = 0;
double currentTime = 0;
double lastTime = 0;
int fps = 0;
int frameCount = 0;
Weapons playerCurrentWeapon = Weapons::DAGGER;
playerAttackPosition attackPositionsArrayDagger[daggerAttacksPosAmount];
playerAttackPosition attackPositionsArraySword[swordAttacksPosAmount];
playerAttackPosition attackPositionsArrayAxe[axeAttacksPosAmount];
playerAttackPosition attackPositionsArrayPole[poleAttacksPosAmount];
playerAttackPosition attackPositionsArrayPoleaxe[poleaxeAttacksPosAmount];