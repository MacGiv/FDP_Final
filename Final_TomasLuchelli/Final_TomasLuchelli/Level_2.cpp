#include "Level_2.h"
#include "Enemy_1.h"

HANDLE hConsole_2 = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap_2[mapSizeRows][mapSizeCols];
cellStruct player_2;

//Weapons playerCurrentWeapon = Weapons::DAGGER;
//attackPosition attackPositionsArray[daggerAttacksPosAmount];
Weapons playerCurrentWeapon = Weapons::SWORD;
playerAttackPosition attackPositionsArray[swordAttacksPosAmount];

// Player's attack variables
double lastAttackTime = 999;
double attackDurationTime = 0.3;
int currentActiveAttacks = 0;
int maxPlayerAttacks = 1;

// Enemies variables
int totalEnemies = 1;
int const maxEnemies = 1;
int const maxEnemyActiveAttacks = maxEnemies * 4; 
enemyAttackPosition enemyAttackPosArray[maxEnemyActiveAttacks];
EnemyCell enemiesArray[maxEnemies];
double lastEnemyMoveTime = 0;
double enemyMoveInterval = 0.7;


bool endLevelConditionMet_2 = false;

void Initialize_2(bool& continueGame, bool& playerLost);
void InitializeEnemies_2();
void Update_2(bool& continueGame);
void ResetPlayerAttackCells();
void ProcessPlayerAttack(AttackDirections attackDir);
void DrawPlayer_2();
void ResetEnemyPrevMovementCell(EnemyCell& tempEnemy);
void DrawUi_2();
void Debug_DrawMap();
void DrawEnemies(EnemyCell enemies[maxEnemies]);

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

    //playerAttackPosition attackPositionsArray[swordAttacksPosAmount];

    do
    {
        CalculateFPS(startTime, currentTime, lastTime, frameCount, fps);
        Update_2(continueGame);
        DrawEnemies(enemiesArray);
        DrawUi_2();
        Sleep(16); // 16 milisecs "=" 64 FPS aprox
    } while (continueGame && !playerLost);
}

void Initialize_2(bool& continueGame, bool& playerLost)
{
    continueGame = true;
    playerLost = false;
    startTime = clock();
    currentTime = 0;
    endLevelConditionMet_2 = false;

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
        break;
    case Weapons::POLE:
        break;
    case Weapons::POLEAXE:
        break;
    default:
        break;
    }

    player_2 = InitializePlayer();


    CreateStandardMap(levelMap_2);
    InitializeEnemies_2();
    levelMap_2[playerStartLevelPosY][playerStartLevelPosX] = player_2;
    Gotoxy(mapStartPosX, mapStartPosY);
    PrintMatrix(levelMap_2, hConsole_2);
}

void InitializeEnemies_2()
{
    for (int i = 0; i < maxEnemies; i++)
    { 
        InitializeEnemy(enemiesArray[i], player_2.posRow, player_2.posCol + (mapSizeCols / 2));
        DrawEnemies(enemiesArray);
        levelMap_2[enemiesArray[i].cell.posRow][enemiesArray[i].cell.posCol].cellType = CellTypes::ENEMY;
    }
}

void Update_2(bool& continueGame)
{
    bool playerHasMoved = false;
    bool playerHasAttacked = false;
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

    // If player moved, update position
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
        // Restore empty cell if player moved through one
        else
        {
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellType = CellTypes::WALKABLE;
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar = charEmpty;
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            cout << charEmpty;
        }
        // Draw player in new position
        DrawPlayer_2();
        playerHasMoved = false;
    }
    else if (playerHasAttacked && currentActiveAttacks < maxPlayerAttacks)
    {
        ProcessPlayerAttack(attackDir);
        playerHasAttacked = false;
        lastAttackTime = currentTime;
        // // ################### Enemy movement ###################
        // for (int i = 0; i < maxEnemies; i++)
        // {
        //     EnemyMoveUp(enemiesArray[i], levelMap_2);
        // }
        // Debug_DrawMap();
        // // ######################################################
        
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
        Debug_DrawMap();
    }

    // Restore attack cells
    if (currentTime > lastAttackTime + attackDurationTime && currentActiveAttacks == maxPlayerAttacks)
    {
        ResetPlayerAttackCells();
    }

    // Check victory condition
    if (player_2.posRow == exitLevelPosY && player_2.posCol == exitLevelPosX)
    {
        continueGame = false;
    }
}

void DrawUi_2()
{
    // Draw UI
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level: " << 2;

    Gotoxy(objetiveInfoPosX, objetiveInfoPosY);
    cout << "Objective: Move the player to the exit using WASD. Attack with IJKL. Press ESC to exit.";

    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";


    Gotoxy(fpsInfoPosX, fpsInfoPosY);
    cout << "FPS: " << static_cast<int>(fps) << "   ";

    //DEBUG
    Debug_DrawMap();
    //END DEBUG

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

void DrawEnemies(EnemyCell enemies[maxEnemies])
{
    for (int i = 0; i < totalEnemies; i++)
    {
        if (enemies[i].isAlive)
        {
            DrawEnemy(enemies[i], levelMap_2, hConsole_2);
            if (enemies[i].hasMoved)
            {
                enemies[i].hasMoved = false;
                ResetEnemyPrevMovementCell(enemies[i]);
                enemies[i].cell.prevPosRow = enemies[i].cell.posRow;
                enemies[i].cell.prevPosCol = enemies[i].cell.posCol;
            }
        }
    }
}

void ResetEnemyPrevMovementCell(EnemyCell& tempEnemy)
{
    // Reset empty cell
    levelMap_2[tempEnemy.cell.prevPosRow][tempEnemy.cell.prevPosCol].cellType = CellTypes::WALKABLE;
    SetConsoleTextAttribute(hConsole_2, colorWalkable);
    Gotoxy(mapStartPosX + tempEnemy.cell.prevPosCol, mapStartPosY + tempEnemy.cell.prevPosRow);
    cout << charEmpty;
    SetConsoleTextAttribute(hConsole_2, colorWall);
}

void ResetPlayerAttackCells()
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
                SetConsoleTextAttribute(hConsole_2, colorWall);
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
                SetConsoleTextAttribute(hConsole_2, colorWall);
            }
        }
        break;
    case Weapons::AXE:
        break;
    case Weapons::POLE:
        break;
    case Weapons::POLEAXE:
        break;
    default:
        break;
    }  
    currentActiveAttacks = 0;
}

void ProcessPlayerAttack(AttackDirections attackDir)
{
    switch (playerCurrentWeapon)
    {
    case Weapons::DAGGER:
        PlayerAttackDagger(levelMap_2, attackDir, attackPositionsArray, player_2);
        //Draw Attack
        for (int i = 0; i < daggerAttacksPosAmount; i++)
        {
            if (attackPositionsArray[i].attackPossible)
            {
                SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charPlayerAttack;
                SetConsoleTextAttribute(hConsole_2, colorWall);
            }
        }
        currentActiveAttacks = daggerAttacksPosAmount;
        break;
    case Weapons::SWORD:
        PlayerAttackSword(levelMap_2, attackDir, attackPositionsArray, player_2);
        //Draw Attack
        for (int i = 0; i < swordAttacksPosAmount; i++)
        {
            if (attackPositionsArray[i].attackPossible)
            {
                SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
                Gotoxy(mapStartPosX + attackPositionsArray[i].col, mapStartPosY + attackPositionsArray[i].row);
                cout << charPlayerAttack;
                SetConsoleTextAttribute(hConsole_2, colorWall);
            }
        }
        currentActiveAttacks = swordAttacksPosAmount;
        break;
    case Weapons::AXE:
        break;
    case Weapons::POLE:
        break;
    case Weapons::POLEAXE:
        break;
    default:
        break;
    }
}

void Debug_DrawMap()
{
    Gotoxy(fpsInfoPosX, fpsInfoPosY + 2);
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