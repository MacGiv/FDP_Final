#include "Weapons.h"

// These functions get the positions of the attacks depending on the weapon used.

void GetAttackPositionsDagger(playerAttackPosition attackPos[daggerAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
	playerAttackPosition auxPos;
	int i = 0;
	
	for (i = 0; i < daggerAttacksPosAmount; i++)
	{
		switch (direction)
		{
		case AttackDirections::NORTH:
			auxPos.row = playerStruct.posRow - 1;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::SOUTH:
			auxPos.row = playerStruct.posRow + 1;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::EAST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol + 1;
			break;
		case AttackDirections::WEST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol - 1;
			break;
		default:
			break;
		}

		attackPos[i] = auxPos;
	}
	
}

void GetAttackPositionsSword(playerAttackPosition attackPos[swordAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
	playerAttackPosition auxPos;
	int i = 0;

	for (i = 0; i < swordAttacksPosAmount; i++)
	{
		switch (direction)
		{
		case AttackDirections::NORTH:
			auxPos.row = playerStruct.posRow - 1 - i;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::SOUTH:
			auxPos.row = playerStruct.posRow + 1 + i;
			auxPos.col = playerStruct.posCol;
			break;
		case AttackDirections::EAST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol + 1 + i;
			break;
		case AttackDirections::WEST:
			auxPos.row = playerStruct.posRow;
			auxPos.col = playerStruct.posCol - 1 - i; 
			break;
		default:
			break;
		}

		attackPos[i] = auxPos;
	}
}

void GetAttackPositionsAxe(playerAttackPosition attackPos[axeAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
    playerAttackPosition auxPos;

    for (int i = 0; i < axeAttacksPosAmount; i++)
    {
        switch (direction)
        {
        case AttackDirections::NORTH:
            if (i < 2) // Golpea en línea recta
            {
                auxPos.row = playerStruct.posRow - 1 - i;
                auxPos.col = playerStruct.posCol;
            }
            else if (i == 2) // Golpe arriba del más lejano
            {
                auxPos.row = playerStruct.posRow - 2;
                auxPos.col = playerStruct.posCol - 1;
            }
            else if (i == 3) // Golpe abajo del más lejano
            {
                auxPos.row = playerStruct.posRow - 2;
                auxPos.col = playerStruct.posCol + 1;
            }
            break;
        case AttackDirections::SOUTH:
            if (i < 2)
            {
                auxPos.row = playerStruct.posRow + 1 + i;
                auxPos.col = playerStruct.posCol;
            }
            else if (i == 2)
            {
                auxPos.row = playerStruct.posRow + 3;
                auxPos.col = playerStruct.posCol - 1;
            }
            else if (i == 3)
            {
                auxPos.row = playerStruct.posRow + 3;
                auxPos.col = playerStruct.posCol + 1;
            }
            break;
        case AttackDirections::EAST:
            if (i < 2)
            {
                auxPos.row = playerStruct.posRow;
                auxPos.col = playerStruct.posCol + 1 + i;
            }
            else if (i == 2)
            {
                auxPos.row = playerStruct.posRow - 1;
                auxPos.col = playerStruct.posCol + 3;
            }
            else if (i == 3)
            {
                auxPos.row = playerStruct.posRow + 1;
                auxPos.col = playerStruct.posCol + 3;
            }
            break;
        case AttackDirections::WEST:
            if (i < 2)
            {
                auxPos.row = playerStruct.posRow;
                auxPos.col = playerStruct.posCol - 1 - i;
            }
            else if (i == 2)
            {
                auxPos.row = playerStruct.posRow - 1;
                auxPos.col = playerStruct.posCol - 3;
            }
            else if (i == 3)
            {
                auxPos.row = playerStruct.posRow + 1;
                auxPos.col = playerStruct.posCol - 3;
            }
            break;
        default:
            break;
        }

        attackPos[i] = auxPos;
    }
}

void GetAttackPositionsPole(playerAttackPosition attackPos[poleAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
    playerAttackPosition auxPos;

    for (int i = 0; i < poleAttacksPosAmount; i++)
    {
        switch (direction)
        {
        case AttackDirections::NORTH:
            auxPos.row = playerStruct.posRow - 1 - i;
            auxPos.col = playerStruct.posCol;
            break;
        case AttackDirections::SOUTH:
            auxPos.row = playerStruct.posRow + 1 + i;
            auxPos.col = playerStruct.posCol;
            break;
        case AttackDirections::EAST:
            auxPos.row = playerStruct.posRow;
            auxPos.col = playerStruct.posCol + 1 + i;
            break;
        case AttackDirections::WEST:
            auxPos.row = playerStruct.posRow;
            auxPos.col = playerStruct.posCol - 1 - i;
            break;
        default:
            break;
        }

        attackPos[i] = auxPos;
    }
}

void GetAttackPositionsPoleaxe(playerAttackPosition attackPos[poleaxeAttacksPosAmount], AttackDirections direction, cellStruct playerStruct)
{
    playerAttackPosition auxPos;

    for (int i = 0; i < poleaxeAttacksPosAmount; i++)
    {
        switch (direction)
        {
        case AttackDirections::NORTH:
            if (i < 3) // Golpea en línea recta
            {
                auxPos.row = playerStruct.posRow - 1 - i;
                auxPos.col = playerStruct.posCol;
            }
            else if (i == 3) // Golpe extra arriba del más lejano
            {
                auxPos.row = playerStruct.posRow - 4;
                auxPos.col = playerStruct.posCol - 1;
            }
            else if (i == 4) // Golpe extra abajo del más lejano
            {
                auxPos.row = playerStruct.posRow - 4;
                auxPos.col = playerStruct.posCol + 1;
            }
            break;
        case AttackDirections::SOUTH:
            if (i < 3)
            {
                auxPos.row = playerStruct.posRow + 1 + i;
                auxPos.col = playerStruct.posCol;
            }
            else if (i == 3)
            {
                auxPos.row = playerStruct.posRow + 4;
                auxPos.col = playerStruct.posCol - 1;
            }
            else if (i == 4)
            {
                auxPos.row = playerStruct.posRow + 4;
                auxPos.col = playerStruct.posCol + 1;
            }
            break;
        case AttackDirections::EAST:
            if (i < 3)
            {
                auxPos.row = playerStruct.posRow;
                auxPos.col = playerStruct.posCol + 1 + i;
            }
            else if (i == 3)
            {
                auxPos.row = playerStruct.posRow - 1;
                auxPos.col = playerStruct.posCol + 4;
            }
            else if (i == 4)
            {
                auxPos.row = playerStruct.posRow + 1;
                auxPos.col = playerStruct.posCol + 4;
            }
            break;
        case AttackDirections::WEST:
            if (i < 3)
            {
                auxPos.row = playerStruct.posRow;
                auxPos.col = playerStruct.posCol - 1 - i;
            }
            else if (i == 3)
            {
                auxPos.row = playerStruct.posRow - 1;
                auxPos.col = playerStruct.posCol - 4;
            }
            else if (i == 4)
            {
                auxPos.row = playerStruct.posRow + 1;
                auxPos.col = playerStruct.posCol - 4;
            }
            break;
        default:
            break;
        }

        attackPos[i] = auxPos;
    }
}
