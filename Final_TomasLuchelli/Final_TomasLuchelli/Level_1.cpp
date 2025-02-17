#include "Level_1.h"
#include <ctime>  // Para medir el tiempo con clock()

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap[mapSizeRows][mapSizeCols];
cellStruct player;
bool endLevelConditionMet = false;
double startTime, currentTime;

void InitializeLevel_1(bool& continueGame, bool& playerLost);
void UpdateLevel_1(bool& continueGame);
void DrawLevel_1();

void StartLevel_1(bool& continueGame, bool& playerLost)
{
    srand(time(0));
    HideCursor();
    InitializeLevel_1(continueGame, playerLost);

    do
    {
        currentTime = (clock() - startTime) / CLOCKS_PER_SEC; //Update time elapsed
        UpdateLevel_1(continueGame);
        DrawLevel_1();
    } while (continueGame && !playerLost);
}

void InitializeLevel_1(bool& continueGame, bool& playerLost)
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

void UpdateLevel_1(bool& continueGame)
{
    bool playerHasMoved = false;
    int inputChar = 0;

    // Get Player input
    if (_kbhit())
    {
        inputChar = _getch();
        if (inputChar != 27)
        {
            ProcessPlayerMovement(playerHasMoved, player, levelMap, inputChar);
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

void DrawLevel_1()
{
    // Draw UI
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level: " << 1;

    Gotoxy(objetiveInfoPosX, objetiveInfoPosY);
    cout << "Objective: Move the player to the exit using WASD. Press ESC to exit.";

    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";
}
