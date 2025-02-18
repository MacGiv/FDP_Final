#include "Utilities.h"
#include "GameData.h"

void Gotoxy(int posX, int posY)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwPos;
	dwPos.X = posX;
	dwPos.Y = posY;

	SetConsoleCursorPosition(hCon, dwPos);
}

//Hide cursor
void HideCursor()
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = false;

	SetConsoleCursorInfo(hCon, &cci);
}

bool IsCorrectInput(int& userInput)
{
	bool isCorrect = true;

	if (!cin)
		isCorrect = false;

	cin.clear();
	cin.ignore(INT_MAX, '\n');

	return isCorrect;
}

bool IsCorrectInput(char& userInput)
{
	bool isCorrect = true;

	if (!cin)
		isCorrect = false;

	cin.clear();
	cin.ignore(INT_MAX, '\n');

	return isCorrect;
}

bool IsCorrectInput(string& userInput)
{

	cin.clear();
	cin.ignore(INT_MAX, '\n');

	return true;
}

void CreateStandardMap(cellStruct myMap[mapSizeRows][mapSizeCols])
{
	for (int i = 0; i < mapSizeRows; i++)
	{
		for (int j = 0; j < mapSizeCols; j++)
		{
			if (i == 0 || i == mapSizeRows - 1)
			{
				if (i == 0 && j == 0)
				{
					myMap[i][j].cellChar = charWallTL;
					myMap[i][j].cellType = CellTypes::WALL;
				}
				else if (i == 0 && j == mapSizeCols - 1)
				{
					myMap[i][j].cellChar = charWallTR;
					myMap[i][j].cellType = CellTypes::WALL;
				}
				else if (i == mapSizeRows - 1 && j == 0)
				{
					myMap[i][j].cellChar = charWallBL;
					myMap[i][j].cellType = CellTypes::WALL;
				}
				else if (i == mapSizeRows - 1 && j == mapSizeCols - 1)
				{
					myMap[i][j].cellChar = charWallBR;
					myMap[i][j].cellType = CellTypes::WALL;
				}
				else
				{
					myMap[i][j].cellChar = charWallH;
					myMap[i][j].cellType = CellTypes::WALL;
				}

			}
			else if (j == 0 || j == mapSizeCols - 1)
			{
				myMap[i][j].cellChar = charWallV;
				myMap[i][j].cellType = CellTypes::WALL;
			}
			else
			{
				myMap[i][j].cellChar = charEmpty;
				myMap[i][j].cellType = CellTypes::WALKABLE;
			}
		}
	}
	//Set Exit
	cellStruct exitCell;
	exitCell.cellChar = charExit;
	exitCell.cellType = CellTypes::EXIT;
	myMap[exitLevelPosY][exitLevelPosX] = exitCell;
}

void PrintMatrix(cellStruct myMatrix[mapSizeRows][mapSizeCols], HANDLE& hConsole)
{

	for (int i = 0; i < mapSizeRows; i++)
	{
		for (int j = 0; j < mapSizeCols; j++)
		{
			if (i == 0 || i == mapSizeRows - 1)
			{
				if (i == 0 && j == 0) //Case top left corner
				{
					SetConsoleTextAttribute(hConsole, colorWall);
					cout << myMatrix[i][j].cellChar;
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (i == 0 && j == mapSizeCols - 1) //Case top right corner
				{
					SetConsoleTextAttribute(hConsole, colorWall);
					cout << myMatrix[i][j].cellChar;
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (i == mapSizeRows - 1 && j == 0) //Case bottom left corner
				{
					SetConsoleTextAttribute(hConsole, colorWall);
					cout << myMatrix[i][j].cellChar;
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (i == mapSizeRows - 1 && j == mapSizeCols - 1) //Case bottom right corner
				{
					SetConsoleTextAttribute(hConsole, colorWall);
					cout << myMatrix[i][j].cellChar;
					SetConsoleTextAttribute(hConsole, 7);
				}
				else
				{
					SetConsoleTextAttribute(hConsole, colorWall); //Case vertical walls
					cout << myMatrix[i][j].cellChar;
					SetConsoleTextAttribute(hConsole, 7);
				}

			}
			else if (j == 0 || j == mapSizeCols - 1) //Case horizontal walls
			{
				SetConsoleTextAttribute(hConsole, colorWall);
				cout << myMatrix[i][j].cellChar;
				SetConsoleTextAttribute(hConsole, 7);
			}
			else if (myMatrix[i][j].cellType == CellTypes::EXIT)
			{
				SetConsoleTextAttribute(hConsole, colorExit);
				cout << myMatrix[i][j].cellChar;
				SetConsoleTextAttribute(hConsole, 7);
			}
			else //If not wall -> Walkable map cases
			{
				SetConsoleTextAttribute(hConsole, colorWalkable);
				cout << myMatrix[i][j].cellChar;
				SetConsoleTextAttribute(hConsole, 7);
			}
		}
		cout << "\n";
		for (int k = 0; k < mapStartPosX; k++)
		{
			cout << " ";
		}
	}
}


double GetTimeElapsed(double startTime)
{
	double now = clock();
	double result = (now - startTime) / 1000.0f;
	return result;
}

//Returns the cellType of a position given ('posRow' & 'posCol') of a matrix ('map')
CellTypes GetCellType(cellStruct map[mapSizeRows][mapSizeCols], int posRow, int posCol)
{
	switch (map[posRow][posCol].cellType)
	{
	case CellTypes::WALKABLE:
		return CellTypes::WALKABLE;
		break;
	case CellTypes::WALL:
		return CellTypes::WALL;
		break;
	case CellTypes::ENEMY:
		return CellTypes::ENEMY;
		break;
	case CellTypes::EXIT:
		return CellTypes::EXIT;
		break;
	default:
		return CellTypes::DEFAULT;
		break;
	}
}