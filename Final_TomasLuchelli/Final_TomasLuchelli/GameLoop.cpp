#include "GameLoop.h"
#include "Level_1.h"
#include "Level_2.h"
#include "Level_3.h"

void StartGame()
{
	bool continueGame = true;
	bool playerLost = false;

	do
	{
		if (!playerLost && playerPhase == 1)
		{
			StartLevel_1(continueGame, playerLost);
		}

		if (!playerLost && playerPhase == 2)
		{
			StartLevel_2(continueGame, playerLost);
		}
		
		if (!playerLost && playerPhase == 3)
		{
			StartLevel_3(continueGame, playerLost);
		}
		
		if (playerPhase == 0)
		{
			continueGame = false;
		}
		
	} while (!continueGame);



}