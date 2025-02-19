#include "Level_1.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
cellStruct levelMap[mapSizeRows][mapSizeCols];
cellStruct player;
bool endLevelConditionMet = false;
double startTime = 0;
double currentTime = 0;
double lastTime = 0;    
int fps = 0;            
int frameCount = 0;    

void Initialize(bool& continueGame, bool& playerLost);
void Update(bool& continueGame);
void Draw();

void StartLevel_1(bool& continueGame, bool& playerLost)
{
    srand(time(0));
    HideCursor();
    Initialize(continueGame, playerLost);
    lastTime = clock();

    do
    {
        CalculateFPS();
        Update(continueGame);
        Draw();
        Sleep(16); // 16 milisecs "=" 64 FPS aprox
    } while (continueGame && !playerLost);
}

void Initialize(bool& continueGame, bool& playerLost)
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

void Update(bool& continueGame)
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

void Draw()
{
    // Draw UI
    Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
    cout << "Level: " << 1;

    Gotoxy(objetiveInfoPosX, objetiveInfoPosY);
    cout << "Objective: Move the player to the exit using WASD. Press ESC to exit.";

    // Draw time elapsed
    Gotoxy(timeInfoPosX, timeInfoPosY);
    cout << "Time: " << static_cast<int>(currentTime) << " sec";


    Gotoxy(fpsInfoPosX, fpsInfoPosY);
    cout << "FPS: " << static_cast<int>(fps) << "   ";
}

void CalculateFPS()
{
    // CLOCKS_PER_SEC = Time measured in processor cycles
    currentTime = (clock() - startTime) / CLOCKS_PER_SEC; // Update time elapsed in seconds
    frameCount++;
    double elapsedTime = (clock() - lastTime) / CLOCKS_PER_SEC; // Calculate elapsed time since the last frame measurement
    if (elapsedTime >= 1.0)
    {
        fps = frameCount;
        frameCount = 0;     // Reset frame counter
        lastTime = clock(); // Update base time
    }

}
