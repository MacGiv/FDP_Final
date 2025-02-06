#include "Level_1.h"


//Level 1
//Condition to win: Get to the exit.

void StartLevel_1(bool& continueGame) 
{
	srand(time(0));
	HideCursor();

	continueGame = true;
	int levelNumber = 1;
	double startTime = clock();
	double currentTime = clock();
	bool endLevelConditionMet = false;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cellStruct levelMap[mapSizeRows][mapSizeCols];
	cellStruct player = InitializePlayer();

	CreateStandardMap(levelMap);
	levelMap[playerStartLevelPosY][playerStartLevelPosX] = player;
	Gotoxy(mapStartPosX, mapStartPosY);
	PrintMatrix(levelMap,hConsole);

	//TODO: Draw UI
	
	//Print level number
	Gotoxy(levelNumberInfoPosX, levelNumberInfoPosY);
	cout << "Level: " << levelNumber;
	//Print objetive
	Gotoxy(objetiveInfoPosX, objetiveInfoPosY);
	cout << "Objetive: " << "Move the player to the exit using WASD.";
	
	do
	{
		bool playerHasMoved = false;
		int inputChar = 0;

		//Get input
		if (_kbhit)
		{
			inputChar = _getch();
			ProcessPlayerMovement(playerHasMoved, player, levelMap, inputChar);

		}

		//If player moved Draw player 
		if (playerHasMoved)
		{
			//If previous cell was ExitCell and player didn't met the condition put back to ExitCell
			if (player.prevPosRow == exitLevelPosY && player.prevPosCol == exitLevelPosX)
			{
				Gotoxy(mapStartPosX + player.prevPosCol, mapStartPosY + player.prevPosRow);
				levelMap[player.prevPosRow][player.prevPosCol].cellType = CellTypes::EXIT;
				levelMap[player.prevPosRow][player.prevPosCol].cellChar = charExit;
				SetConsoleTextAttribute(hConsole, colorExit);
				cout << levelMap[player.prevPosRow][player.prevPosCol].cellChar;
				SetConsoleTextAttribute(hConsole, 7);
			}
			else //Else = cell was a normal walkable one. Set normal walkable tile.
			{
				Gotoxy(mapStartPosX + player.prevPosCol, mapStartPosY + player.prevPosRow);
				SetConsoleTextAttribute(hConsole, colorWalkable);
				cout << charEmpty;
				SetConsoleTextAttribute(hConsole, 7);
				levelMap[player.prevPosRow][player.prevPosCol].cellType = CellTypes::WALKABLE;
				levelMap[player.prevPosRow][player.prevPosCol].cellChar = charEmpty;
			}

			//Draw player pos
			Gotoxy(mapStartPosX + player.posCol, mapStartPosY + player.posRow);
			SetConsoleTextAttribute(hConsole, colorPlayer);
			cout << player.cellChar;
			SetConsoleTextAttribute(hConsole, 7);
			levelMap[player.posRow][player.posCol].cellType = CellTypes::PLAYER;
			levelMap[player.posRow][player.posCol].cellChar = charPlayer;

			playerHasMoved = false;
		}

		//Check win level condition
		if (player.posRow == exitLevelPosY && player.posCol == exitLevelPosX)
		{
			continueGame = false;
		}


	} while (continueGame);
}

