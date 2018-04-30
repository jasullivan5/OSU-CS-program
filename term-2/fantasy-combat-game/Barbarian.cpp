/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Barbarian class implementation file.
**              Barbarian Attack: 2d6, Defense: 2d6, Armor: 0, Strength: 12
*********************************************************************/

#include "Barbarian.hpp"

/*********************************************************************
** Constructor
*********************************************************************/
Barbarian::Barbarian() {
	name = "Barbarian";
	armor = 0;
	totalStrength = 12;
	strengthLeft = 12;
	attDiceSides = 6;
	attDiceNum = 2;
	defDiceSides = 6;
	defDiceNum = 2;
}

/*********************************************************************
** Getname morphed to fullfill abstract base class requirement.
*********************************************************************/
string Barbarian::getName()
{
	return "Barbarian";
}
