#include "Level_2.h"

HANDLE hConsole_2 = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap_2[mapSizeRows][mapSizeCols];
cellStruct player_2;

//Weapons playerCurrentWeapon = Weapons::DAGGER;
//attackPosition attackPositionsArray[daggerAttacksPosAmount];
Weapons playerCurrentWeapon = Weapons::SWORD;
attackPosition attackPositionsArray[swordAttacksPosAmount];

double lastAttackTime = 999;
double attackDurationTime = 0.3;
int currentActiveAttacks = 0;
const int maxPlayerAttacks = 1;

bool endLevelConditionMet_2 = false;

void Initialize_2(bool& continueGame, bool& playerLost);
void Update_2(bool& continueGame);
void Draw_2();

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
        Update_2(continueGame);
        Draw_2();
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

    player_2 = InitializePlayer();
    CreateStandardMap(levelMap_2);
    levelMap_2[playerStartLevelPosY][playerStartLevelPosX] = player_2;

    Gotoxy(mapStartPosX, mapStartPosY);
    PrintMatrix(levelMap_2, hConsole_2);
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
            SetConsoleTextAttribute(hConsole_2, colorWalkable);
            cout << charEmpty;
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellType = CellTypes::WALKABLE;
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar = charEmpty;
        }
        // Draw player in new position
        Gotoxy(mapStartPosX + player_2.posCol, mapStartPosY + player_2.posRow);
        SetConsoleTextAttribute(hConsole_2, colorPlayer);
        cout << player_2.cellChar;
        levelMap_2[player_2.posRow][player_2.posCol].cellType = CellTypes::PLAYER;
        levelMap_2[player_2.posRow][player_2.posCol].cellChar = charPlayer;
        SetConsoleTextAttribute(hConsole_2, colorWall);
        playerHasMoved = false;
    }
    else if (playerHasAttacked && currentActiveAttacks < maxPlayerAttacks)
    {
        PlayerAttack(levelMap_2, attackDir, attackPositionsArray, player_2);
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
        currentActiveAttacks = 1;
        playerHasAttacked = false;
        lastAttackTime = currentTime;
    }

    // Restore attack cells
    if (currentTime > lastAttackTime + attackDurationTime && currentActiveAttacks == maxPlayerAttacks)
    {
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
        currentActiveAttacks = 0;
    }

    // Check victory condition
    if (player_2.posRow == exitLevelPosY && player_2.posCol == exitLevelPosX)
    {
        continueGame = false;
    }
}

void Draw_2()
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

}


