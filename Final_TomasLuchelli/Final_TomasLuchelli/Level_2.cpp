#include "Level_2.h"
#include "Enemy_1.h"

HANDLE hConsole_2 = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap_2[mapSizeRows][mapSizeCols];

cellStruct player_2;
//Weapons playerCurrentWeapon = Weapons::DAGGER;
//attackPosition attackPositionsArray[daggerAttacksPosAmount];
//Weapons playerCurrentWeapon = Weapons::SWORD;
//playerAttackPosition attackPositionsArray[swordAttacksPosAmount];
//Weapons playerCurrentWeapon = Weapons::AXE;
//playerAttackPosition attackPositionsArray[axeAttacksPosAmount];
Weapons playerCurrentWeapon = Weapons::POLE;
playerAttackPosition attackPositionsArray[poleAttacksPosAmount];
int playerHp = 100;
bool hitReceived = false;

// Player's attack variables
extern double lastAttackTime = 999;
extern double attackDurationTime = 0.3;
extern int    currentActiveAttacks = 0;
extern int    maxPlayerAttacks = 1;
extern bool   playerHasAttacked = false;

// Enemies variables
int enemyDamage = 50;
int totalEnemies = 1;
int const maxEnemies = 1;
int const maxEnemyActiveAttacks = maxEnemyAttacksP1; 
enemyAttackPosition enemyAttackPosArray[maxEnemyActiveAttacks];
EnemyCell enemiesArray[maxEnemies];
double lastEnemyMoveTime = 0;
double enemyMoveInterval = 0.7;
double lastEnemyAttackTime = 1;
double enemyAttackInterval = 1;

bool endLevelConditionMet_2 = false;

void Initialize_2(bool& continueGame, bool& playerLost);
void InitializeEnemies_2();
void Update_2(bool& continueGame, bool& playerLost, bool& playerHasAttacked);
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
void DrawEnemies_2(EnemyCell enemies[maxEnemies]);
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
    
    Initialize_2(continueGame, playerLost);

    do
    {
        CalculateFPS(startTime, currentTime, lastTime, frameCount, fps);
        Update_2(continueGame, playerLost, playerHasAttacked);
        Draw_2();
        Sleep(16);
    } while (continueGame && !playerLost);
}

void Initialize_2(bool& continueGame, bool& playerLost)
{
    continueGame = true;
    playerLost = false;
    startTime = clock();
    currentTime = 0;
    endLevelConditionMet_2 = false;
    playerHp = 100;
    hitReceived = false;
    lastEnemyMoveTime = 0;
    enemyMoveInterval = 0.7;
    lastEnemyAttackTime = 1;
    enemyAttackInterval = 1;
    // Initialize attack positions array
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        maxPlayerAttacks = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        maxPlayerAttacks = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        maxPlayerAttacks = axeAttacksPosAmount;
        break;
    case Weapons::POLE:
        maxPlayerAttacks = poleAttacksPosAmount;
        break;
    case Weapons::POLEAXE:
        maxPlayerAttacks = poleaxeAttacksPosAmount;
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
    for (int i = 0; i < maxPlayerAttacks; i++)
    {
        enemyAttackPosArray[i].row = 0;
        enemyAttackPosArray[i].col = 0;
        enemyAttackPosArray[i].isActive = false;
    }
    for (int i = 0; i < maxEnemies; i++)
    { 
        InitializeEnemy(enemiesArray[i], player_2.posRow, player_2.posCol + (mapSizeCols / 2));
        levelMap_2[enemiesArray[i].cell.posRow][enemiesArray[i].cell.posCol].cellType = CellTypes::ENEMY;
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
            if (IsMovementInput(inputChar))
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
            continueGame = false;
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
    else if (playerHasAttacked && currentActiveAttacks < maxPlayerAttacks)
    {
        ProcessPlayerAttack_2(attackDir);
        CheckEnemyHit_2();
        lastAttackTime = currentTime;        
    }

    // Reset enemy attack cells
    if (currentTime > lastEnemyAttackTime + attackDurationTime)
    {
        ResetEnemyAttackCells_2();
        // Prevent morre than 1 hits to Player in the same attack
        if (hitReceived == true)
        {
            hitReceived = false;
        }
    }
    // Reset player attack cells
    if (currentTime > lastAttackTime + attackDurationTime && currentActiveAttacks == maxPlayerAttacks)
    {
        ResetPlayerAttackCells_2();
    }

    // Enemy Move
    currentTime = (clock() - startTime) / static_cast<double>(CLOCKS_PER_SEC);
    if (currentTime > lastEnemyMoveTime + enemyMoveInterval) 
    {
        for (int i = 0; i < maxEnemies; i++) 
        {
            if (enemiesArray[i].isAlive) 
            {
                MoveEnemy(enemiesArray[i], levelMap_2);
            }
        }
        lastEnemyMoveTime = currentTime;
        //Debug_DrawMap();
    }
    // Enemy Attack
    else if (currentTime > lastEnemyAttackTime + enemyAttackInterval)
    {
        for (int i = 0; i < maxEnemies; i++)
        {
            if (enemiesArray[i].isAlive)
            {
                EnemyAttack(enemiesArray[i], levelMap_2, enemiesArray[i].attackType, enemyAttackPosArray);
            }
        }
        lastEnemyAttackTime = currentTime;
    }

    // Check for player damage
    if (!hitReceived)
    {
        for (int i = 0; i < maxEnemyActiveAttacks; i++)
        {
            if (enemyAttackPosArray[i].isActive && player_2.posRow == enemyAttackPosArray[i].row && player_2.posCol == enemyAttackPosArray[i].col)
            {
                playerHp -= 50;
                hitReceived = true;
                if (playerHp <= 0)
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
        for (int i = 0; i < maxEnemies; i++)
        {
            if (enemiesArray[i].isAlive)
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

void ProcessPlayerAttack_2(AttackDirections attackDir)
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        PlayerAttackDagger(levelMap_2, attackDir, attackPositionsArray, player_2);
        currentActiveAttacks = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        PlayerAttackSword(levelMap_2, attackDir, attackPositionsArray, player_2);
        currentActiveAttacks = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        PlayerAttackAxe(levelMap_2, attackDir, attackPositionsArray, player_2);
        currentActiveAttacks = axeAttacksPosAmount;
        break;
    case Weapons::POLE:
        PlayerAttackPole(levelMap_2, attackDir, attackPositionsArray, player_2);
        currentActiveAttacks = poleAttacksPosAmount;
        break;
    case Weapons::POLEAXE:
        PlayerAttackPoleaxe(levelMap_2, attackDir, attackPositionsArray, player_2);
        currentActiveAttacks = poleaxeAttacksPosAmount;
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
    for (int i = 0; i < maxEnemyActiveAttacks; i++)
    {
        if (enemyAttackPosArray[i].isActive)
        {
            enemyAttackPosArray[i].isActive = false;
            levelMap_2[enemyAttackPosArray[i].row][enemyAttackPosArray[i].col].cellType = CellTypes::WALKABLE;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            Gotoxy(mapStartPosX + enemyAttackPosArray[i].col, mapStartPosY + enemyAttackPosArray[i].row);
            cout << charEmpty;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            enemyAttackPosArray[i].row = 0;
            enemyAttackPosArray[i].col = 0;
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
            if (attackPositionsArray[i].attackPossible)
            {
                levelMap_2[attackPositionsArray[i].row][attackPositionsArray[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::SWORD:
        for (int i = 0; i < swordAttacksPosAmount; i++)
        {
            if (attackPositionsArray[i].attackPossible)
            {
                levelMap_2[attackPositionsArray[i].row][attackPositionsArray[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::AXE:
        for (int i = 0; i < axeAttacksPosAmount; i++)
        {
            if (attackPositionsArray[i].attackPossible)
            {
                levelMap_2[attackPositionsArray[i].row][attackPositionsArray[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::POLE:
        for (int i = 0; i < poleAttacksPosAmount; i++)
        {
            if (attackPositionsArray[i].attackPossible)
            {
                levelMap_2[attackPositionsArray[i].row][attackPositionsArray[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    case Weapons::POLEAXE:
        for (int i = 0; i < poleaxeAttacksPosAmount; i++)
        {
            if (attackPositionsArray[i].attackPossible)
            {
                levelMap_2[attackPositionsArray[i].row][attackPositionsArray[i].col].cellType = CellTypes::WALKABLE;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charEmpty;
                SetConsoleTextAttribute(hConsole_2, colorWalkable);
            }
        }
        break;
    default:
        break;
    }  
    currentActiveAttacks = 0;
}

void CheckEnemyHit_2() 
{
    for (int i = 0; i < maxPlayerAttacks; i++) 
    {
        if (attackPositionsArray[i].attackPossible)
        {
            for (int j = 0; j < maxEnemies; j++) 
            {
                if (enemiesArray[j].isAlive)
                {
                    if (attackPositionsArray[i].row == enemiesArray[j].cell.posRow &&
                        attackPositionsArray[i].col == enemiesArray[j].cell.posCol) 
                    {

                        enemiesArray[j].hp -= 100;

                        if (enemiesArray[j].hp <= 0) 
                        {
                            enemiesArray[j].isAlive = false;
                            levelMap_2[enemiesArray[j].cell.posRow]
                                      [enemiesArray[j].cell.posCol].cellType = CellTypes::WALKABLE;
                        }
                    }
                }
                
            }
        }
    }
}

void DrawEnemyAttack_2()
{
    for (int i = 0; i < maxEnemyActiveAttacks; i++)
    {
        if (enemyAttackPosArray[i].isActive)
        {
            SetConsoleTextAttribute(hConsole_2, colorEnemyAttack);
            Gotoxy(mapStartPosX + enemyAttackPosArray[i].col, mapStartPosY + enemyAttackPosArray[i].row);
            cout << charEnemyAttack;
            SetConsoleTextAttribute(hConsole_2, colorWalkable); 
        }
    }
}

void Draw_2()
{
    DrawPlayer_2();
    DrawEnemies_2(enemiesArray);
    DrawEnemyAttack_2();

    if (playerHasAttacked)
    {
        DrawPlayerAttack_2();
    }

    // Draw UI
    SetConsoleTextAttribute(hConsole_2, colorWall);
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level 2";
    // Draw level info
    Gotoxy(objetiveInfoPosX, objetiveInfoPosY);
    cout << "Objective: Move the player to the exit using WASD. Attack with IJKL. Press ESC to exit.";

    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";
    // Draw FPS
    Gotoxy(fpsInfoPosX, fpsInfoPosY);
    cout << "FPS: " << static_cast<int>(fps) << "   ";
    // Draw Player's HP
    if (playerHp < 100)
    {
        SetConsoleTextAttribute(hConsole_2, colorEnemy);
        Gotoxy(playerHpPosX, playerHpPosY);
        cout << "HP:  " << playerHp;
    }
    else
    {
        Gotoxy(playerHpPosX, playerHpPosY);
        cout << "HP: " << playerHp;
    }
    

    //DEBUG
    Debug_DrawMap_2();
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

void DrawEnemies_2(EnemyCell enemies[maxEnemies])
{
    for (int i = 0; i < totalEnemies; i++)
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
        if (attackPositionsArray[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
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
        if (attackPositionsArray[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
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
        if (attackPositionsArray[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
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
        if (attackPositionsArray[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
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
        if (attackPositionsArray[i].attackPossible)
        {
            SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
            Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
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