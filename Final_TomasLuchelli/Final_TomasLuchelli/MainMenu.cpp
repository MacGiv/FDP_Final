#include "MainMenu.h"
#include "Utilities.h"
#include "GameLoop.h"


void ShowMainMenu()
{
	bool continueProgram = true;
	
	do
	{
		int inputOption = 0;
		bool validInput = false;

		system("cls");
		cout << "MAIN MENU\n";
		cout << "Welcome to the game!\n";
		cout << "1. Play\n";
		cout << "2. How to play\n";
		cout << "3. Credits\n";
		cout << "4. Exit\n";
		cout << "Enter an option:\n";
		cin >> inputOption;

		do
		{
			
			if (IsCorrectInput(inputOption) && (inputOption >= 1 && inputOption <= 4) )
			{
				validInput = true;
			}
			else
			{
				validInput = false;
				cout << "Invalid input, reenter: \n";
				cin >> inputOption;
			}
		} while (!validInput);

		switch (static_cast<MenuOptions>(inputOption))
		{
		case MenuOptions::PLAY:
			
			StartGame();

			break;
		case MenuOptions::HOW_TO_PLAY:

			break;
		case MenuOptions::CREDITS:
			system("cls");
			cout << "Examen Final \n";
			cout << "Fundamentos de la Programacion - Laboratorio de Programacion\n";
			cout << "Tomas Francisco Luchelli\n";
			cout << "31-07-2024\n\n";
			system("pause");
			break;
		case MenuOptions::EXIT:
			continueProgram = false;
			break;
		default:
			break;
		}
	} while (continueProgram);

}