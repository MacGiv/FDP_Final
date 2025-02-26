#include "Level_2.h"

HANDLE hConsole_2 = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap_2[mapSizeRows][mapSizeCols];
cellStruct player_2;

Weapons playerCurrentWeapon = Weapons::DAGGER;
int attackPositionsArray[daggerAttacksPosAmount * 2];

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

        if (player_2.prevPosRow == exitLevelPosY && player_2.prevPosCol == exitLevelPosX)
        {
            // Restore exit cell if player moved through
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellType = CellTypes::EXIT;
            levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar = charExit;
            SetConsoleTextAttribute(hConsole_2, colorExit);
            cout << levelMap_2[player_2.prevPosRow][player_2.prevPosCol].cellChar;
        }
        else
        {
            // Restore empty cell if player moved through one
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
    else if (playerHasAttacked)
    {
        PlayerDaggerAttack(levelMap_2, playerCurrentWeapon, attackDir, attackPositionsArray, player_2);
        //Draw Attack
        SetConsoleTextAttribute(hConsole_2, colorPlayerAttack);
        Gotoxy(mapStartPosX + attackPositionsArray[1], mapStartPosY + attackPositionsArray[0]);
        cout << charPlayerAttack;
        SetConsoleTextAttribute(hConsole_2, colorWall);
        playerHasAttacked = false;
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
    cout << "Objective: Move the player to the exit using WASD. Press ESC to exit.";

    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";


    Gotoxy(fpsInfoPosX, fpsInfoPosY);
    cout << "FPS: " << static_cast<int>(fps) << "   ";

}


