#pragma once

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <string>

#include "GameData.h"

using namespace std;

//Hide cursor
void HideCursor();

//Gotoxy
void Gotoxy(int posX, int posY);

//Correct input checkers
bool IsCorrectInput(int& userInput);
bool IsCorrectInput(char& userInput);
bool IsCorrectInput(string& userInput);

//Create map with walls and walkables
void CreateStandardMap(cellStruct myMap[mapSizeRows][mapSizeCols]);

//First print for each level
void PrintMatrix(cellStruct myMatrix[mapSizeRows][mapSizeCols], HANDLE& hConsole);

//Returns the time elapsed from startTime until the moment of the function call
double GetTimeElapsed(double startTime);

//Returns the cellType of a position given ('posRow' & 'posCol') of a matrix ('map')
CellTypes GetCellType(cellStruct map[mapSizeRows][mapSizeCols], int posRow, int posCol);

void CalculateFPS(double& startTime, double& currentTime, double& lastTime, int& frameCount, int& fps);

void CleanHead();