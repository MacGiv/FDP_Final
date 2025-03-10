#include "Level_3.h"
#include "Enemy_1.h"

HANDLE hConsole_3 = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap_3[mapSizeRows][mapSizeCols];

cellStruct player_3;

int playerHp_3 = 200;
bool hitReceived_3 = false;

// Player's attack variables
double lastAttackTime_3 = 999;
double attackDurationTime_3 = 0.3;
int currentActiveAttacks_3 = 0;
int maxPlayerAttacks_3 = 1;
bool playerHasAttacked_3 = false;

// Enemies variables
int enemyDamage_3 = 50;
int const maxEnemies_3 = 4;
int const maxEnemyActiveAttacks_3 = maxEnemyAttacksP3;
enemyAttackPosition enemyAttackPosArray_3[maxEnemyActiveAttacks_3];
EnemyCell enemiesArray_3[maxEnemies_3];
double lastEnemyMoveTime_3 = 0;
double enemyMoveInterval_3 = 0.7;
double lastEnemyAttackTime_3 = 1;
double enemyAttackInterval_3 = 1;

bool endLevelConditionMet_3 = false;

void Initialize_3(bool& continueGame, bool& playerLost);
void InitializeEnemies_3();
void Update_3(bool& continueGame, bool& playerLost, bool& playerHasAttacked);
void SwitchWeapon_3(int inputChar);
void ProcessPlayerAttack_3(AttackDirections attackDir);
void CheckEnemyHit_3();
void ResetPlayerAttackCells_3();
void ResetEnemyPrevMovementCell_3(EnemyCell& tempEnemy);
void ResetEnemyAttackCells_3();
void DrawEnemyAttack_3();
void DrawPlayerSwordAttack_3();
void DrawPlayerDaggerAttack_3();
void DrawPlayerAxeAttack_3();
void DrawPlayerPoleAttack_3();
void DrawPlayerPoleaxeAttack_3();
void DrawPlayer_3();
void Draw_3();
void DrawPlayerAttack_3();
void DrawEnemies_3(EnemyCell enemies[maxEnemies_3]);
void Debug_DrawMap_3();

void StartLevel_3(bool& continueGame, bool& playerLost)
{
    srand(time(0));
    HideCursor();
    startTime = 0;
    currentTime = 0;
    lastTime = clock();
    fps = 0;
    frameCount = 0;
    CleanHead();
    Initialize_3(continueGame, playerLost);

    do
    {
        CalculateFPS(startTime, currentTime, lastTime, frameCount, fps);
        Update_3(continueGame, playerLost, playerHasAttacked_3);
        Draw_3();
        Sleep(16);
    } while (continueGame && !playerLost);
    if (!playerLost)
    {
        CleanHead();
        Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
        cout << "You WIN!";
        // Draw level info
        Gotoxy(infoOnePosX, infoOnePosY);
        cout << "Congrats.";
        Sleep(1000);
        playerPhase = 0;
    }
    else
    {
        playerPhase = 0;
    }
}

void Initialize_3(bool& continueGame, bool& playerLost)
{
    continueGame = true;
    playerLost = false;
    startTime = clock();
    currentTime = 0;
    endLevelConditionMet_3 = false;
    playerHp_3 = 100;
    hitReceived_3 = false;
    lastEnemyMoveTime_3 = 0;
    lastEnemyAttackTime_3 = 1;
    // Initialize attack positions array
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        maxPlayerAttacks_3 = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        maxPlayerAttacks_3 = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        maxPlayerAttacks_3 = axeAttacksPosAmount;
        break;
    case Weapons::POLE:
        maxPlayerAttacks_3 = poleAttacksPosAmount;
        break;
    case Weapons::POLEAXE:
        maxPlayerAttacks_3 = poleaxeAttacksPosAmount;
        break;
    default:
        break;
    }

    CreateStandardMap(levelMap_3);
    player_3 = InitializePlayer();
    InitializeEnemies_3();
    levelMap_3[playerStartLevelPosY][playerStartLevelPosX] = player_3;
    Gotoxy(mapStartPosX, mapStartPosY);
    PrintMatrix(levelMap_3, hConsole_3);
}

void InitializeEnemies_3()
{
    for (int i = 0; i < maxPlayerAttacks_3; i++)
    {
        enemyAttackPosArray_3[i].row = 0;
        enemyAttackPosArray_3[i].col = 0;
        enemyAttackPosArray_3[i].isActive = false;
    }
    for (int i = 0; i < maxEnemies_3; i++)
    {
        int randTemp = rand() % 4 + 2;  // Between 2 (doesn't hit the player) and 4 (not out of bounds)
        InitializeEnemy(enemiesArray_3[i], mapCenterPosY, mapOneFifthPosX * randTemp, i);
        levelMap_3[enemiesArray_3[i].cell.posRow][enemiesArray_3[i].cell.posCol].cellType = CellTypes::ENEMY;
    }
}

void Update_3(bool& continueGame, bool& playerLost, bool& playerHasAttacked)
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
                ResetPlayerAttackCells_3();
                int tempValue = static_cast<int>(inputChar) - 48;
                SwitchWeapon_3(tempValue);
            }
            else if (IsMovementInput(inputChar))
            {
                ProcessPlayerMovement(playerHasMoved, player_3, levelMap_3, inputChar);
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
            continueGame = false;
        }
    }

    // Player Movement
    if (playerHasMoved)
    {
        Gotoxy(mapStartPosX + player_3.prevPosCol, mapStartPosY + player_3.prevPosRow);

        // Restore exit cell if player moved through
        if (player_3.prevPosRow == exitLevelPosY && player_3.prevPosCol == exitLevelPosX)
        {
            levelMap_3[player_3.prevPosRow][player_3.prevPosCol].cellType = CellTypes::EXIT;
            levelMap_3[player_3.prevPosRow][player_3.prevPosCol].cellChar = charExit;
            SetConsoleTextAttribute(hConsole_3, colorExit);
            cout << levelMap_3[player_3.prevPosRow][player_3.prevPosCol].cellChar;
        }
        // Restore empty cell if player moved through
        else
        {
            levelMap_3[player_3.prevPosRow][player_3.prevPosCol].cellType = CellTypes::WALKABLE;
            levelMap_3[player_3.prevPosRow][player_3.prevPosCol].cellChar = charEmpty;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
            cout << charEmpty;
        }
        playerHasMoved = false;
    }
    // Player Attack
    else if (playerHasAttacked && currentActiveAttacks_3 < maxPlayerAttacks_3)
    {
        ProcessPlayerAttack_3(attackDir);
        CheckEnemyHit_3();
        lastAttackTime_3 = currentTime;
    }

    // Reset enemy attack cells
    if (currentTime > lastEnemyAttackTime_3 + attackDurationTime_3)
    {
        ResetEnemyAttackCells_3();
        // Prevent more than 1 hit to Player in the same attack
        if (hitReceived_3 == true)
        {
            hitReceived_3 = false;
        }
    }
    // Reset player attack cells
    if (currentTime > lastAttackTime_3 + attackDurationTime_3 && currentActiveAttacks_3 == maxPlayerAttacks_3)
    {
        ResetPlayerAttackCells_3();
    }

    // Enemy Move
    currentTime = (clock() - startTime) / static_cast<double>(CLOCKS_PER_SEC);
    if (currentTime > lastEnemyMoveTime_3 + enemyMoveInterval_3)
    {
        for (int i = 0; i < maxEnemies_3; i++)
        {
            if (enemiesArray_3[i].isAlive)
            {
                MoveEnemy(enemiesArray_3[i], levelMap_3);
            }
        }
        lastEnemyMoveTime_3 = currentTime;
    }
    // Enemy Attack
    else if (currentTime > lastEnemyAttackTime_3 + enemyAttackInterval_3)
    {
        for (int i = 0; i < maxEnemies_3; i++)
        {
            if (enemiesArray_3[i].isAlive)
            {
                EnemyAttack(enemiesArray_3[i], levelMap_3, enemiesArray_3[i].attackType, enemyAttackPosArray_3);
            }
        }
        lastEnemyAttackTime_3 = currentTime;
    }

    // Check for player damage
    if (!hitReceived_3)
    {
        for (int i = 0; i < maxEnemyActiveAttacks_3; i++)
        {
            if (enemyAttackPosArray_3[i].isActive && player_3.posRow == enemyAttackPosArray_3[i].row && player_3.posCol == enemyAttackPosArray_3[i].col)
            {
                playerHp_3 -= 50;
                hitReceived_3 = true;
                if (playerHp_3 <= 0)
                {
                    playerLost = true;
                    ResetEnemyAttackCells_3();
                }
            }
        }
    }

    // Check victory condition
    if (player_3.posRow == exitLevelPosY && player_3.posCol == exitLevelPosX)
    {
        int count = 0;
        for (int i = 0; i < maxEnemies_3; i++)
        {
            if (enemiesArray_3[i].isAlive)
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

void SwitchWeapon_3(int weaponSelected)
{
    switch (weaponSelected)
    {
    case static_cast<int>(Weapons::DAGGER):
        maxPlayerAttacks_3 = daggerAttacksPosAmount;
        playerCurrentWeapon = Weapons::DAGGER;
        break;
    case static_cast<int>(Weapons::SWORD):
        maxPlayerAttacks_3 = swordAttacksPosAmount;
        playerCurrentWeapon = Weapons::SWORD;
        break;
    case static_cast<int>(Weapons::AXE):
        maxPlayerAttacks_3 = axeAttacksPosAmount;
        playerCurrentWeapon = Weapons::AXE;
        break;
    case static_cast<int>(Weapons::POLE):
        maxPlayerAttacks_3 = poleAttacksPosAmount;
        playerCurrentWeapon = Weapons::POLE;
        break;
    case static_cast<int>(Weapons::POLEAXE):
        maxPlayerAttacks_3 = poleaxeAttacksPosAmount;
        playerCurrentWeapon = Weapons::POLEAXE;
        break;
    default:
        break;
    }
}

void ProcessPlayerAttack_3(AttackDirections attackDir)
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        PlayerAttackDagger(levelMap_3, attackDir, attackPositionsArrayDagger, player_3);
        currentActiveAttacks_3 = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        PlayerAttackSword(levelMap_3, attackDir, attackPositionsArraySword, player_3);
        currentActiveAttacks_3 = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        PlayerAttackAxe(levelMap_3, attackDir, attackPositionsArrayAxe, player_3);
        currentActiveAttacks_3 = axeAttacksPosAmount;
        break;
    case Weapons::POLE:
        PlayerAttackPole(levelMap_3, attackDir, attackPositionsArrayPole, player_3);
        currentActiveAttacks_3 = poleAttacksPosAmount;
        break;
    case Weapons::POLEAXE:
        PlayerAttackPoleaxe(levelMap_3, attackDir, attackPositionsArrayPoleaxe, player_3);
        currentActiveAttacks_3 = poleaxeAttacksPosAmount;
        break;
    default:
        break;
    }
}

void ResetEnemyPrevMovementCell_3(EnemyCell& tempEnemy)
{
    // Reset empty cell
    levelMap_3[tempEnemy.cell.prevPosRow][tempEnemy.cell.prevPosCol].cellType = CellTypes::WALKABLE;
    SetConsoleTextAttribute(hConsole_3, colorWalkable);
    Gotoxy(mapStartPosX + tempEnemy.cell.prevPosCol, mapStartPosY + tempEnemy.cell.prevPosRow);
    cout << charEmpty;
    SetConsoleTextAttribute(hConsole_3, colorWalkable);
}

void ResetEnemyAttackCells_3()
{
    for (int i = 0; i < maxEnemyActiveAttacks_3; i++)
    {
        if (enemyAttackPosArray_3[i].isActive)
        {
            enemyAttackPosArray_3[i].isActive = false;
            levelMap_3[enemyAttackPosArray_3[i].row][enemyAttackPosArray_3[i].col].cellType = CellTypes::WALKABLE;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
            Gotoxy(mapStartPosX + enemyAttackPosArray_3[i].col, mapStartPosY + enemyAttackPosArray_3[i].row);
            cout << charEmpty;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
            enemyAttackPosArray_3[i].row = 0;
            enemyAttackPosArray_3[i].col = 0;
        }
    }
}

void ResetPlayerAttackCells_3()
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        for (int i = 0; i < daggerAttacksPosAmount; i++)
        {
            if (attackPositionsArrayDagger[i].attackPossible)
            {
                levelMap_3[attackPositionsArrayDagger[i].row][attackPositionsArrayDagger[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayDagger[i].col, mapStartPosY + attackPositionsArrayDagger[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
            }
        }
        break;
    case Weapons::SWORD:
        for (int i = 0; i < swordAttacksPosAmount; i++)
        {
            if (attackPositionsArraySword[i].attackPossible)
            {
                levelMap_3[attackPositionsArraySword[i].row][attackPositionsArraySword[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArraySword[i].col, mapStartPosY + attackPositionsArraySword[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
            }
        }
        break;
    case Weapons::AXE:
        for (int i = 0; i < axeAttacksPosAmount; i++)
        {
            if (attackPositionsArrayAxe[i].attackPossible)
            {
                levelMap_3[attackPositionsArrayAxe[i].row][attackPositionsArrayAxe[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayAxe[i].col, mapStartPosY + attackPositionsArrayAxe[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
            }
        }
        break;
    case Weapons::POLE:
        for (int i = 0; i < poleAttacksPosAmount; i++)
        {
            if (attackPositionsArrayPole[i].attackPossible)
            {
                levelMap_3[attackPositionsArrayPole[i].row][attackPositionsArrayPole[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayPole[i].col, mapStartPosY + attackPositionsArrayPole[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
            }
        }
        break;
    case Weapons::POLEAXE:
        for (int i = 0; i < poleaxeAttacksPosAmount; i++)
        {
            if (attackPositionsArrayPoleaxe[i].attackPossible)
            {
                levelMap_3[attackPositionsArrayPoleaxe[i].row][attackPositionsArrayPoleaxe[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArrayPoleaxe[i].col, mapStartPosY + attackPositionsArrayPoleaxe[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_3, colorWalkable);
            }
        }
        break;
    default:
        break;
    }
    currentActiveAttacks_3 = 0;
}

void CheckEnemyHit_3()
{
    for (int i = 0; i < maxPlayerAttacks_3; i++)
    {
        switch (playerCurrentWeapon)
        {
        case Weapons::DAGGER:
            if (attackPositionsArrayDagger[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_3; j++)
                {
                    if (enemiesArray_3[j].isAlive)
                    {
                        if (attackPositionsArrayDagger[i].row == enemiesArray_3[j].cell.posRow &&
                            attackPositionsArrayDagger[i].col == enemiesArray_3[j].cell.posCol)
                        {

                            enemiesArray_3[j].hp -= 100;

                            if (enemiesArray_3[j].hp <= 0)
                            {
                                enemiesArray_3[j].isAlive = false;
                                levelMap_3[enemiesArray_3[j].cell.posRow]
                                    [enemiesArray_3[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::SWORD:
            if (attackPositionsArraySword[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_3; j++)
                {
                    if (enemiesArray_3[j].isAlive)
                    {
                        if (attackPositionsArraySword[i].row == enemiesArray_3[j].cell.posRow &&
                            attackPositionsArraySword[i].col == enemiesArray_3[j].cell.posCol)
                        {

                            enemiesArray_3[j].hp -= 100;

                            if (enemiesArray_3[j].hp <= 0)
                            {
                                enemiesArray_3[j].isAlive = false;
                                levelMap_3[enemiesArray_3[j].cell.posRow]
                                    [enemiesArray_3[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::AXE:
            if (attackPositionsArrayAxe[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_3; j++)
                {
                    if (enemiesArray_3[j].isAlive)
                    {
                        if (attackPositionsArrayAxe[i].row == enemiesArray_3[j].cell.posRow &&
                            attackPositionsArrayAxe[i].col == enemiesArray_3[j].cell.posCol)
                        {

                            enemiesArray_3[j].hp -= 100;

                            if (enemiesArray_3[j].hp <= 0)
                            {
                                enemiesArray_3[j].isAlive = false;
                                levelMap_3[enemiesArray_3[j].cell.posRow]
                                    [enemiesArray_3[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::POLE:
            if (attackPositionsArrayPole[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_3; j++)
                {
                    if (enemiesArray_3[j].isAlive)
                    {
                        if (attackPositionsArrayPole[i].row == enemiesArray_3[j].cell.posRow &&
                            attackPositionsArrayPole[i].col == enemiesArray_3[j].cell.posCol)
                        {

                            enemiesArray_3[j].hp -= 100;

                            if (enemiesArray_3[j].hp <= 0)
                            {
                                enemiesArray_3[j].isAlive = false;
                                levelMap_3[enemiesArray_3[j].cell.posRow]
                                    [enemiesArray_3[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        case Weapons::POLEAXE:
            if (attackPositionsArrayPoleaxe[i].attackPossible)
            {
                for (int j = 0; j < maxEnemies_3; j++)
                {
                    if (enemiesArray_3[j].isAlive)
                    {
                        if (attackPositionsArrayPoleaxe[i].row == enemiesArray_3[j].cell.posRow &&
                            attackPositionsArrayPoleaxe[i].col == enemiesArray_3[j].cell.posCol)
                        {

                            enemiesArray_3[j].hp -= 100;

                            if (enemiesArray_3[j].hp <= 0)
                            {
                                enemiesArray_3[j].isAlive = false;
                                levelMap_3[enemiesArray_3[j].cell.posRow]
                                    [enemiesArray_3[j].cell.posCol].cellType = CellTypes::WALKABLE;
                            }
                        }
                    }

                }
            }
            break;
        default:
            break;
        }
    }
}

void DrawEnemyAttack_3()
{
    for (int i = 0; i < maxEnemyActiveAttacks_3; i++)
    {
        if (enemyAttackPosArray_3[i].isActive)
        {
            SetConsoleTextAttribute(hConsole_3, colorEnemyAttack);
            Gotoxy(mapStartPosX + enemyAttackPosArray_3[i].col, mapStartPosY + enemyAttackPosArray_3[i].row);
            cout << charEnemyAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void Draw_3()
{
    DrawPlayer_3();
    DrawEnemies_3(enemiesArray_3);
    DrawEnemyAttack_3();

    if (playerHasAttacked_3)
    {
        DrawPlayerAttack_3();
    }

    // Draw UI
    SetConsoleTextAttribute(hConsole_3, colorWall);
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level 3";
    // Draw level info
    Gotoxy(infoOnePosX, infoOnePosY);
    cout << "Objective: Defeat the enemies and get to the exit.";
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
    if (playerHp_3 < 100)
    {
        SetConsoleTextAttribute(hConsole_3, colorEnemy);
        Gotoxy(playerHpPosX, playerHpPosY);
        cout << "HP:  " << playerHp_3;
    }
    else
    {
        Gotoxy(playerHpPosX, playerHpPosY);
        cout << "HP: " << playerHp_3;
    }

    SetConsoleTextAttribute(hConsole_3, colorWalkable);

}

void DrawPlayerAttack_3()
{
    for (int i = 0; i < currentActiveAttacks_3; i++)
    {
        if (attackPositionsArrayDagger[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_3, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayDagger[i].col, mapStartPosY + attackPositionsArrayDagger[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void DrawPlayer_3()
{
    Gotoxy(mapStartPosX + player_3.posCol, mapStartPosY + player_3.posRow);
    SetConsoleTextAttribute(hConsole_3, colorPlayer);
    cout << player_3.cellChar;
    levelMap_3[player_3.posRow][player_3.posCol].cellType = CellTypes::PLAYER;
    levelMap_3[player_3.posRow][player_3.posCol].cellChar = charPlayer;
    SetConsoleTextAttribute(hConsole_3, colorWall);
}

void DrawEnemies_3(EnemyCell enemies[maxEnemies_3])
{
    for (int i = 0; i < maxEnemies_3; i++)
    {
        if (enemies[i].isAlive)
        {
            DrawEnemy(enemies[i], levelMap_3, hConsole_3);
            if (enemies[i].hasMoved)
            {
                enemies[i].hasMoved = false;
                ResetEnemyPrevMovementCell_3(enemies[i]);
                enemies[i].cell.prevPosRow = enemies[i].cell.posRow;
                enemies[i].cell.prevPosCol = enemies[i].cell.posCol;
            }
        }
    }
}


void DrawPlayerSwordAttack_3()
{
    //Draw Attack
    for (int i = 0; i < swordAttacksPosAmount; i++)
    {
        if (attackPositionsArraySword[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_3, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArraySword[i].col, mapStartPosY + attackPositionsArraySword[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void DrawPlayerDaggerAttack_3()
{
    //Draw Attack
    for (int i = 0; i < daggerAttacksPosAmount; i++)
    {
        if (attackPositionsArrayDagger[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_3, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayDagger[i].col, mapStartPosY + attackPositionsArrayDagger[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void DrawPlayerAxeAttack_3()
{
    //Draw Attack
    for (int i = 0; i < axeAttacksPosAmount; i++)
    {
        if (attackPositionsArrayAxe[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_3, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayAxe[i].col, mapStartPosY + attackPositionsArrayAxe[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void DrawPlayerPoleAttack_3()
{
    //Draw Attack
    for (int i = 0; i < poleAttacksPosAmount; i++)
    {
        if (attackPositionsArrayPole[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_3, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayPole[i].col, mapStartPosY + attackPositionsArrayPole[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void DrawPlayerPoleaxeAttack_3()
{
    //Draw Attack
    for (int i = 0; i < poleaxeAttacksPosAmount; i++)
    {
        if (attackPositionsArrayPoleaxe[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_3, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArrayPoleaxe[i].col, mapStartPosY + attackPositionsArrayPoleaxe[i].row);
            cout << charPlayerAttack;
            SetConsoleTextAttribute(hConsole_3, colorWalkable);
        }
    }
}

void Debug_DrawMap_3()
{
    Gotoxy(playerHpPosX, playerHpPosY + 2);
    cout << endl;
    for (int i = 0; i < mapSizeRows; i++)
    {
        for (int j = 0; j < mapSizeCols; j++)
        {
            cout << static_cast<int>(levelMap_3[i][j].cellType);
        }
        cout << endl;
    }
}