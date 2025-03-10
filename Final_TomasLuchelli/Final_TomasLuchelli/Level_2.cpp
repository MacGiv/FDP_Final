#include "Level_2.h"
#include "Enemy_1.h"

HANDLE hConsole_2 = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap_2[mapSizeRows][mapSizeCols];

cellStruct player_2;

int playerHp_2 = 100;
bool hitReceived_2 = false;

// Player's attack variables
double lastAttackTime_2 = 999;
double attackDurationTime_2 = 0.3;
int currentActiveAttacks_2 = 0;
int maxPlayerAttacks_2 = 1;
bool playerHasAttacked_2 = false;

// Enemies variables
int enemyDamage_2 = 50;
int const maxEnemies_2 = 2;
int const maxEnemyActiveAttacks_2 = maxEnemyAttacksP1; 
enemyAttackPosition enemyAttackPosArray_2[maxEnemyActiveAttacks_2];
EnemyCell enemiesArray_2[maxEnemies_2];
double lastEnemyMoveTime_2 = 0;
double enemyMoveInterval_2 = 0.7;
double lastEnemyAttackTime_2 = 1;
double enemyAttackInterval_2 = 1;

bool endLevelConditionMet_2 = false;

void Initialize_2(bool& continueGame, bool& playerLost);
void InitializeEnemies_2();
void Update_2(bool& continueGame, bool& playerLost, bool& playerHasAttacked);
void SwitchWeapon_2(int inputChar);
void ProcessPlayerAttack_2(AttackDirections attackDir);
void CheckEnemyHit_2();
void ResetPlayerAttackCells_2();
void ResetEnemyPrevMovementCell_2(EnemyCell& tempEnemy);
void ResetEnemyAttackCells_2();
void DrawEnemyAttack_2();
void DrawPlayerSwordAttack_2();
void DrawPlayerDaggerAttack_2();
void DrawPlayerAxeAttack_2();
void DrawPlayerPoleAttack_2();
void DrawPlayerPoleaxeAttack_2();
void DrawPlayer_2();
void Draw_2();
void DrawPlayerAttack_2();
void DrawEnemies_2(EnemyCell enemies[maxEnemies_2]);
void Debug_DrawMap_2();

void StartLevel_2(bool& continueGame, bool& playerLost)
{
    srand(time(0));
    HideCursor();
    startTime = 0;
    currentTime = 0;
    lastTime = clock();
    fps = 0;
    frameCount = 0;
    CleanHead();

    Initialize_2(continueGame, playerLost);

    do
    {
        CalculateFPS(startTime, currentTime, lastTime, frameCount, fps);
        Update_2(continueGame, playerLost, playerHasAttacked_2);
        Draw_2();
        Sleep(16);
    } while (continueGame && !playerLost);

    if (!playerLost)
    {
        playerPhase = 3 ;
        continueGame = true;
    }
    else
    {
        playerPhase = 1;
    }
}

void Initialize_2(bool& continueGame, bool& playerLost)
{
    continueGame = true;
    playerLost = false;
    startTime = clock();
    currentTime = 0;
    endLevelConditionMet_2 = false;
    playerHp_2 = 100;
    hitReceived_2 = false;
    lastEnemyMoveTime_2 = 0;
    lastEnemyAttackTime_2 = 1;
    // Initialize attack positions array
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        maxPlayerAttacks_2 = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        maxPlayerAttacks_2 = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        maxPlayerAttacks_2 = axeAttacksPosAmount;
        break;
    case Weapons::POLE:
        maxPlayerAttacks_2 = poleAttacksPosAmount;
        break;
    case Weapons::POLEAXE:
        maxPlayerAttacks_2 = poleaxeAttacksPosAmount;
        break;
    default:
        break;
    }

    CreateStandardMap(levelMap_2);
    player_2 = InitializePlayer();
    InitializeEnemies_2();
    levelMap_2[playerStartLevelPosY][playerStartLevelPosX] = player_2;
    Gotoxy(mapStartPosX, mapStartPosY);
    PrintMatrix(levelMap_2, hConsole_2);
}

void InitializeEnemies_2()
{
    for (int i = 0; i < maxPlayerAttacks_2; i++)
    {
        enemyAttackPosArray_2[i].row = 0;
        enemyAttackPosArray_2[i].col = 0;
        enemyAttackPosArray_2[i].isActive = false;
    }
    for (int i = 0; i < maxEnemies_2; i++)
    { 
        int randTemp = rand() % 4 + 2;  // Between 2 (doesn't hit the player) and 4 (not out of bounds)
        InitializeEnemy(enemiesArray_2[i], mapCenterPosY, mapOneFifthPosX*randTemp, i);
        levelMap_2[enemiesArray_2[i].cell.posRow][enemiesArray_2[i].cell.posCol].cellType = CellTypes::ENEMY;
    }
}

void Update_2(bool& continueGame, bool& playerLost, bool& playerHasAttacked)
{
    bool playerHasMoved = false;
    playerHasAttacked = false;
    AttackDirections attackDir = AttackDirections::NORTH;
    char inputChar = 0;

    // Get Player input
    if (_kbhit())
    {
        inputChar = _getch();
        if (inputChar != charEscapeKey)
        {
            if (IsWeaponInput(inputChar) && !playerHasAttacked)
            {
                ResetPlayerAttackCells_2();
                int tempValue = static_cast<int>(inputChar) - 48;
                SwitchWeapon_2(tempValue);
            }
            else if (IsMovementInput(inputChar))
            {
                ProcessPlayerMovement(playerHasMoved, player_2, levelMap_2, inputChar);
            }
            else if (IsAttackInput(inputChar))
            {
                attackDir = GetAttackDirection(inputChar);
                playerHasAttacked = true;
            }
        }
        else
        {
            playerHasMoved = false;
        }
    }

    // Player Movement
    if (playerHasMoved)
    {
        Gotoxy(mapStartPosX + player_2.prevPosCol, mapStartPosY + player_2.prevPosRow);

        // Restore exit cell if player moved through
        if (player_2.prevPosRow == exitLevelPosY && player_2.prevPosCol == exitLevelPosX)
        {
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellType = CellTypes::EXIT;
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar = charExit;
            SetConsoleTextAttribute(hConsole_2, colorExit);
            cout << levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar;
        }
        // Restore empty cell if player moved through
        else
        {
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellType = CellTypes::WALKABLE;
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar = charEmpty;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            cout << charEmpty;
        }
        playerHasMoved = false;
    }
    // Player Attack
    else if (playerHasAttacked && currentActiveAttacks_2 < maxPlayerAttacks_2)
    {
        ProcessPlayerAttack_2(attackDir);
        CheckEnemyHit_2();
        lastAttackTime_2 = currentTime;        
    }

    // Reset enemy attack cells
    if (currentTime > lastEnemyAttackTime_2 + attackDurationTime_2)
    {
        ResetEnemyAttackCells_2();
        // Prevent morre than 1 hits to Player in the same attack
        if (hitReceived_2 == true)
        {
            hitReceived_2 = false;
        }
    }
    // Reset player attack cells
    if (currentTime > lastAttackTime_2 + attackDurationTime_2 && currentActiveAttacks_2 == maxPlayerAttacks_2)
    {
        ResetPlayerAttackCells_2();
    }

    // Enemy Move
    currentTime = (clock() - startTime) / static_cast<double>(CLOCKS_PER_SEC);
    if (currentTime > lastEnemyMoveTime_2 + enemyMoveInterval_2) 
    {
        for (int i = 0; i < maxEnemies_2; i++) 
        {
            if (enemiesArray_2[i].isAlive) 
            {
                MoveEnemy(enemiesArray_2[i], levelMap_2);
            }
        }
        lastEnemyMoveTime_2 = currentTime;
        //Debug_DrawMap();
    }
    // Enemy Attack
    else if (currentTime > lastEnemyAttackTime_2 + enemyAttackInterval_2)
    {
        for (int i = 0; i < maxEnemies_2; i++)
        {
            if (enemiesArray_2[i].isAlive)
            {
                EnemyAttack(enemiesArray_2[i], levelMap_2, enemiesArray_2[i].attackType, enemyAttackPosArray_2);
            }
        }
        lastEnemyAttackTime_2 = currentTime;
    }

    // Check for player damage
    if (!hitReceived_2)
    {
        for (int i = 0; i < maxEnemyActiveAttacks_2; i++)
        {
            if (enemyAttackPosArray_2[i].isActive && player_2.posRow == enemyAttackPosArray_2[i].row && player_2.posCol == enemyAttackPosArray_2[i].col)
            {
                playerHp_2 -= 50;
                hitReceived_2 = true;
                if (playerHp_2 <= 0)
                {
                    playerLost = true;
                    ResetEnemyAttackCells_2();
                }
            }
        }
    }

    // Check victory condition
    if (player_2.posRow == exitLevelPosY && player_2.posCol == exitLevelPosX)
    {
        int count = 0;
        for (int i = 0; i < maxEnemies_2; i++)
        {
            if (enemiesArray_2[i].isAlive)
            {
                count++;
            }
        }
        if (count == 0)
        {
            continueGame = false;
        }
    }
}

void SwitchWeapon_2(int weaponSelected)
{
    switch (weaponSelected)
    {
    case static_cast<int>(Weapons::DAGGER):
        maxPlayerAttacks_2 = daggerAttacksPosAmount;
        playerCurrentWeapon = Weapons::DAGGER;
        break;
    case static_cast<int>(Weapons::SWORD):
        maxPlayerAttacks_2 = swordAttacksPosAmount;
        playerCurrentWeapon = Weapons::SWORD;
        break;
    case static_cast<int>(Weapons::AXE):
        maxPlayerAttacks_2 = axeAttacksPosAmount;
        playerCurrentWeapon = Weapons::AXE;
        break;
    case static_cast<int>(Weapons::POLE):
        maxPlayerAttacks_2 = poleAttacksPosAmount;
        playerCurrentWeapon = Weapons::POLE;
        break;
    case static_cast<int>(Weapons::POLEAXE):
        maxPlayerAttacks_2 = poleaxeAttacksPosAmount;
        playerCurrentWeapon = Weapons::POLEAXE;
        break;
    default:
        break;
    }
}

void ProcessPlayerAttack_2(AttackDirections attackDir)
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        PlayerAttackDagger(levelMap_2, attackDir, attackPositionsArrayDagger, player_2);
        currentActiveAttacks_2 = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        PlayerAttackSword(levelMap_2, attackDir, attackPositionsArraySword, player_2);
        currentActiveAttacks_2 = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        PlayerAttackAxe(levelMap_2, attackDir, attackPositionsArrayAxe, player_2);
        currentActiveAttacks_2 = axeAttacksPosAmount;
        break;
    case Weapons::POLE:
        PlayerAttackPole(levelMap_2, attackDir, attackPositionsArrayPole, player_2);
        currentActiveAttacks_2 = poleAttacksPosAmount;
        break;
    case Weapons::POLEAXE:
        PlayerAttackPoleaxe(levelMap_2, attackDir, attackPositionsArrayPoleaxe, player_2);
        currentActiveAttacks_2 = poleaxeAttacksPosAmount;
        break;
    default:
        break;
    }
}

void ResetEnemyPrevMovementCell_2(EnemyCell& tempEnemy)
{
    // Reset empty cell
    levelMap_2[tempEnemy.cell.prevPosRow][tempEnemy.cell.prevPosCol].cellType = CellTypes::WALKABLE;
    SetConsoleTextAttribute(hConsole_2, colorWalkable);
    Gotoxy(mapStartPosX + tempEnemy.cell.prevPosCol, mapStartPosY + tempEnemy.cell.prevPosRow);
    cout << charEmpty;
    SetConsoleTextAttribute(hConsole_2, colorWalkable);
}

void ResetEnemyAttackCells_2()
{
    for (int i = 0; i < maxEnemyActiveAttacks_2; i++)
    {
        if (enemyAttackPosArray_2[i].isActive)
        {
            enemyAttackPosArray_2[i].isActive = false;
            levelMap_2[enemyAttackPosArray_2[i].row][enemyAttackPosArray_2[i].col].cellType = CellTypes::WALKABLE;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            Gotoxy(mapStartPosX + enemyAttackPosArray_2[i].col, mapStartPosY + enemyAttackPosArray_2[i].row);
            cout << charEmpty;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            enemyAttackPosArray_2[i].row = 0;
            enemyAttackPosArray_2[i].col = 0;
        }
    }
}

void ResetPlayerAttackCells_2()
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        for (int i = 0; i < daggerAttacksPosAmount; i++)
        {
            if (attackPositionsArrayDagger[i].attackPossible)
            {
                levelMap_2[attackPositionsArrayDagger[i].row][attackPositionsArrayDagger[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayDagger[i].col, mapStartPosY + attackPositionsArrayDagger[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::SWORD:
        for (int i = 0; i < swordAttacksPosAmount; i++)
        {
            if (attackPositionsArraySword[i].attackPossible)
            {
                levelMap_2[attackPositionsArraySword[i].row][attackPositionsArraySword[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArraySword[i].col, mapStartPosY + attackPositionsArraySword[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::AXE:
        for (int i = 0; i < axeAttacksPosAmount; i++)
        {
            if (attackPositionsArrayAxe[i].attackPossible)
            {
                levelMap_2[attackPositionsArrayAxe[i].row][attackPositionsArrayAxe[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayAxe[i].col, mapStartPosY + attackPositionsArrayAxe[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::POLE:
        for (int i = 0; i < poleAttacksPosAmount; i++)
        {
            if (attackPositionsArrayPole[i].attackPossible)
            {
                levelMap_2[attackPositionsArrayPole[i].row][attackPositionsArrayPole[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayPole[i].col, mapStartPosY + attackPositionsArrayPole[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::POLEAXE:
        for (int i = 0; i < poleaxeAttacksPosAmount; i++)
        {
            if (attackPositionsArrayPoleaxe[i].attackPossible)
            {
                levelMap_2[attackPositionsArrayPoleaxe[i].row][attackPositionsArrayPoleaxe[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayPoleaxe[i].col, mapStartPosY + attackPositionsArrayPoleaxe[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    default:
        break;
    }  
    currentActiveAttacks_2 = 0;
}

void CheckEnemyHit_2() 
{
    for (int i = 0; i < maxPlayerAttacks_2; i++) 
    {
        switch (playerCurrentWeapon)
        {
        case Weapons::DAGGER:
        if (attackPositionsArrayDagger[i].attackPossible)
        {
            for (int j = 0; j < maxEnemies_2; j++)
            {
                if (enemiesArray_2[j].isAlive)
                {
                    if (attackPositionsArrayDagger[i].row == enemiesArray_2[j].cell.posRow &&
                        attackPositionsArrayDagger[i].col == enemiesArray_2[j].cell.posCol)
                    {

                        enemiesArray_2[j].hp -= 100;

                        if (enemiesArray_2[j].hp <= 0)
                        {
                            enemiesArray_2[j].isAlive = false;
                            levelMap_2[enemiesArray_2[j].cell.posRow]
                                [enemiesArray_2[j].cell.posCol].cellType = CellTypes::WALKABLE;
                        }
                    }
                }

            }
        }
            break;
        case Weapons::SWORD:
            if (attackPositionsArraySword[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_2; j++)
                {
                    if (enemiesArray_2[j].isAlive)
                    {
                        if (attackPositionsArraySword[i].row == enemiesArray_2[j].cell.posRow &&
                            attackPositionsArraySword[i].col == enemiesArray_2[j].cell.posCol)
                        {

                            enemiesArray_2[j].hp -= 100;

                            if (enemiesArray_2[j].hp <= 0)
                            {
                                enemiesArray_2[j].isAlive = false;
                                levelMap_2[enemiesArray_2[j].cell.posRow]
                                    [enemiesArray_2[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::AXE:
            if (attackPositionsArrayAxe[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_2; j++)
                {
                    if (enemiesArray_2[j].isAlive)
                    {
                        if (attackPositionsArrayAxe[i].row == enemiesArray_2[j].cell.posRow &&
                            attackPositionsArrayAxe[i].col == enemiesArray_2[j].cell.posCol)
                        {

                            enemiesArray_2[j].hp -= 100;

                            if (enemiesArray_2[j].hp <= 0)
                            {
                                enemiesArray_2[j].isAlive = false;
                                levelMap_2[enemiesArray_2[j].cell.posRow]
                                    [enemiesArray_2[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::POLE:
            if (attackPositionsArrayPole[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_2; j++)
                {
                    if (enemiesArray_2[j].isAlive)
                    {
                        if (attackPositionsArrayPole[i].row == enemiesArray_2[j].cell.posRow &&
                            attackPositionsArrayPole[i].col == enemiesArray_2[j].cell.posCol)
                        {

                            enemiesArray_2[j].hp -= 100;

                            if (enemiesArray_2[j].hp <= 0)
                            {
                                enemiesArray_2[j].isAlive = false;
                                levelMap_2[enemiesArray_2[j].cell.posRow]
                                    [enemiesArray_2[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::POLEAXE:
            if (attackPositionsArrayPoleaxe[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_2; j++)
                {
                    if (enemiesArray_2[j].isAlive)
                    {
                        if (attackPositionsArrayPoleaxe[i].row == enemiesArray_2[j].cell.posRow &&
                            attackPositionsArrayPoleaxe[i].col == enemiesArray_2[j].cell.posCol)
                        {

                            enemiesArray_2[j].hp -= 100;

                            if (enemiesArray_2[j].hp <= 0)
                            {
                                enemiesArray_2[j].isAlive = false;
                                levelMap_2[enemiesArray_2[j].cell.posRow]
                                    [enemiesArray_2[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        default:
            break;
        }
        //if (attackPositionsArray[i].attackPossible)
        //{
        //    for (int j = 0; j < maxEnemies; j++) 
        //    {
        //        if (enemiesArray[j].isAlive)
        //        {
        //            if (attackPositionsArray[i].row == enemiesArray[j].cell.posRow /&&
        //                attackPositionsArray[i].col == enemiesArray[j].cell.posCol) 
        //            {
        //
        //                enemiesArray[j].hp -= 100;
        //
        //                if (enemiesArray[j].hp <= 0) 
        //                {
        //                    enemiesArray[j].isAlive = false;
        //                    levelMap_2[enemiesArray[j].cell.posRow]
        //                              [enemiesArray[j].cell.posCol].cellType = //CellTypes::WALKABLE;
        //                }
        //            }
        //        }
        //        
        //    }
        //}
    }
}

void DrawEnemyAttack_2()
{
    for (int i = 0; i < maxEnemyActiveAttacks_2; i++)
    {
        if (enemyAttackPosArray_2[i].isActive)
        {
            SetConsoleTextAttribute(hConsole_2, colorEnemyAttack);
            Gotoxy(mapStartPosX + enemyAttackPosArray_2[i].col, mapStartPosY + enemyAttackPosArray_2[i].row);
            cout << charEnemyAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable); 
        }
    }
}

void Draw_2()
{
    DrawPlayer_2();
    DrawEnemies_2(enemiesArray_2);
    DrawEnemyAttack_2();

    if (playerHasAttacked_2)
    {
        DrawPlayerAttack_2();
    }

    // Draw UI
    SetConsoleTextAttribute(hConsole_2, colorWall);
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level 2";
    // Draw level info
    Gotoxy(infoOnePosX, infoOnePosY);
    cout << "Objective: Attack with I, J, K and L. Defeat the enemy and get to the exit.";
    Gotoxy(infoTwoPosX, infoTwoPosY);
    cout << "1 = Dagger | 2 = Sword | 3 = Axe | 4 = Pole | 5 = Poleaxe";
    Gotoxy(infoThreePosX, infoThreePosY);
    cout << "Change weapons with 1, 2, 3, 4 and 5.";
    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";
    // Draw FPS
    Gotoxy(fpsInfoPosX, fpsInfoPosY);
    cout << "FPS: " << static_cast<int>(fps) << "   ";
    // Draw Player's HP
    if (playerHp_2 < 100)
    {
        SetConsoleTextAttribute(hConsole_2, colorEnemy);
        Gotoxy(playerHpPosX, playerHpPosY);
        cout << "HP:  " << playerHp_2;
    }
    else
    {
        Gotoxy(playerHpPosX, playerHpPosY);
        cout << "HP: " << playerHp_2;
    }
    

    //DEBUG
    //Debug_DrawMap_2();
    //END DEBUG

    SetConsoleTextAttribute(hConsole_2, colorWalkable);

}

void DrawPlayerAttack_2()
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        DrawPlayerDaggerAttack_2();
        break;
    case Weapons::SWORD:
        DrawPlayerSwordAttack_2();
        break;
    case Weapons::AXE:
        DrawPlayerAxeAttack_2();
        break;
    case Weapons::POLE:
        DrawPlayerPoleAttack_2();
        break;
    case Weapons::POLEAXE:
        DrawPlayerPoleaxeAttack_2();
        break;
    default:
        break;
    }
}

void DrawPlayer_2()
{
    Gotoxy(mapStartPosX + player_2.posCol, mapStartPosY + player_2.posRow);
    SetConsoleTextAttribute(hConsole_2, colorPlayer);
    cout << player_2.cellChar;
    levelMap_2[player_2.posRow][player_2.posCol].cellType = CellTypes::PLAYER;
    levelMap_2[player_2.posRow][player_2.posCol].cellChar = charPlayer;
    SetConsoleTextAttribute(hConsole_2, colorWall);
}

void DrawEnemies_2(EnemyCell enemies[maxEnemies_2])
{
    for (int i = 0; i < maxEnemies_2; i++)
    {
        if (enemies[i].isAlive)
        {
            DrawEnemy(enemies[i], levelMap_2, hConsole_2);
            if (enemies[i].hasMoved)
            {
                enemies[i].hasMoved = false;
                ResetEnemyPrevMovementCell_2(enemies[i]);
                enemies[i].cell.prevPosRow = enemies[i].cell.posRow;
                enemies[i].cell.prevPosCol = enemies[i].cell.posCol;
            }
        }
    }
}


void DrawPlayerSwordAttack_2()
{
    //Draw Attack
    for (int i = 0; i < swordAttacksPosAmount; i++)
    {
        if (attackPositionsArraySword[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArraySword[i].col, mapStartPosY + attackPositionsArraySword[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
        }
    }
}

void DrawPlayerDaggerAttack_2()
{
    //Draw Attack
    for (int i = 0; i < daggerAttacksPosAmount; i++)
    {
        if (attackPositionsArrayDagger[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayDagger[i].col, mapStartPosY + attackPositionsArrayDagger[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
        }
    }
}

void DrawPlayerAxeAttack_2()
{
    //Draw Attack
    for (int i = 0; i < axeAttacksPosAmount; i++)
    {
        if (attackPositionsArrayAxe[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayAxe[i].col, mapStartPosY + attackPositionsArrayAxe[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
        }
    }
}

void DrawPlayerPoleAttack_2()
{
    //Draw Attack
    for (int i = 0; i < poleAttacksPosAmount; i++)
    {
        if (attackPositionsArrayPole[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayPole[i].col, mapStartPosY + attackPositionsArrayPole[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
        }
    }
}

void DrawPlayerPoleaxeAttack_2()
{
    //Draw Attack
    for (int i = 0; i < poleaxeAttacksPosAmount; i++)
    {
        if (attackPositionsArrayPoleaxe[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayPoleaxe[i].col, mapStartPosY + attackPositionsArrayPoleaxe[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
        }
    }
}

void Debug_DrawMap_2()
{
    Gotoxy(playerHpPosX, playerHpPosY+ 2);
    cout << endl;
    for (int i = 0; i < mapSizeRows; i++)
    {
        for (int j = 0; j < mapSizeCols; j++)
        {
            cout << static_cast<int>(levelMap_2[i][j].cellType);
        }
        cout << endl;
    }
}

