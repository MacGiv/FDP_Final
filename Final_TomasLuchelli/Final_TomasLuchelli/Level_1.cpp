#include "Level_1.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap[mapSizeRows][mapSizeCols];
cellStruct player;
bool endLevelConditionMet = false;
  

void Initialize_1(bool& continueGame, bool& playerLost);
void Update_1(bool& continueGame, bool& playerLost);
void Draw_1();

void StartLevel_1(bool& continueGame, bool& playerLost)
{
    startTime = 0;
    currentTime = 0;
    lastTime = clock();
    fps = 0;
    frameCount = 0;

    srand(time(0));
    HideCursor();
    Initialize_1(continueGame, playerLost);

    do
    {
        CalculateFPS(startTime, currentTime, lastTime, frameCount, fps);
        Update_1(continueGame, playerLost);
        Draw_1();
        Sleep(16); 
    } while (continueGame && !playerLost);
    playerPhase = 2;
}

void Initialize_1(bool& continueGame, bool& playerLost)
{
    continueGame = true;
    playerLost = false;
    startTime = clock();
    currentTime = 0;
    endLevelConditionMet = false;

    player = InitializePlayer();
    CreateStandardMap(levelMap);
    levelMap[playerStartLevelPosY][playerStartLevelPosX] = player;

    Gotoxy(mapStartPosX, mapStartPosY);
    PrintMatrix(levelMap, hConsole);
}

void Update_1(bool& continueGame, bool& playerLost)
{
    bool playerHasMoved = false;
    int inputChar = 0;

    // Get Player input
    if (_kbhit())
    {
        inputChar = _getch();
        if (inputChar != charEscapeKey)
        {
            ProcessPlayerMovement(playerHasMoved, player, levelMap, inputChar);
        }
        else
        {
            playerHasMoved = false;
            playerLost = true;
        }
    }

    // If player moved, update position
    if (playerHasMoved)
    {
        Gotoxy(mapStartPosX + player.prevPosCol, mapStartPosY + player.prevPosRow);

        if (player.prevPosRow == exitLevelPosY && player.prevPosCol == exitLevelPosX)
        {
            // Restore exit cell if player moved through
            levelMap[player.prevPosRow][player.prevPosCol].cellType = CellTypes::EXIT;
            levelMap[player.prevPosRow][player.prevPosCol].cellChar = charExit;
            SetConsoleTextAttribute(hConsole, colorExit);
            cout << levelMap[player.prevPosRow][player.prevPosCol].cellChar;
        }
        else
        {
            // Restore empty cell if player moved through one
            SetConsoleTextAttribute(hConsole, colorWalkable);
            cout << charEmpty;
            levelMap[player.prevPosRow][player.prevPosCol].cellType = CellTypes::WALKABLE;
            levelMap[player.prevPosRow][player.prevPosCol].cellChar = charEmpty;
        }

        // Draw player in new position
        Gotoxy(mapStartPosX + player.posCol, mapStartPosY + player.posRow);
        SetConsoleTextAttribute(hConsole, colorPlayer);
        cout << player.cellChar;
        levelMap[player.posRow][player.posCol].cellType = CellTypes::PLAYER;
        levelMap[player.posRow][player.posCol].cellChar = charPlayer;
        SetConsoleTextAttribute(hConsole, colorWall);
        playerHasMoved = false;
    }

    // Check victory condition
    if (player.posRow == exitLevelPosY && player.posCol == exitLevelPosX)
    {
        continueGame = false;
    }
}

void Draw_1()
{
    // Draw UI
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level: 1";

    Gotoxy(infoOnePosX, infoOnePosY);
    cout << "Objective: Move the player to the exit using WASD. Press ESC to exit.";

    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";


    Gotoxy(fpsInfoPosX, fpsInfoPosY);
    cout << "FPS: " << static_cast<int>(fps) << "   ";
}


