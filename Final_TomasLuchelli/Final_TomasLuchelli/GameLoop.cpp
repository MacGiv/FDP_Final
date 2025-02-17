#include "GameLoop.h"
#include "Level_1.h"

void StartGame()
{
	bool continueGame = true;

	do
	{
		bool playerAlive = true;

		//Start Level 1 / Tutorial Movement & Exit Point
		system("cls");
		StartLevel_1(continueGame);

		//## Only For debugging ##
		Gotoxy(endLevelMsgPosX, endLevelMsgPosY);
		system("pause");
		//## Only For debugging ##

		//Start Level 2 / Tutorial Attack
		
		//Start Level 3 / Tutorial Traps & Life recover
		
		//Start Level 4 / Tutorial Enemies

		//Start Level 5 / Traps 1
		
		//Start Level 5 / Enemies 1
		
		//Start Level 5 / Traps 2
		
		//Start Level 5 / Enemies 2
		
		//Start Level 6 Boss

	} while (continueGame);



}