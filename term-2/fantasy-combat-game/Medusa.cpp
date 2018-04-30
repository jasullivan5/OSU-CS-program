/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Medusa class implementation file.
**              Medusa can turn opponents to stone with her glare instantly
**              defeating them when she rolls her maximum attack value.
**              Attack: 2d6, Defense: 1d6, Armor: 3, Strength: 8
*********************************************************************/

#include "Medusa.hpp"

/*********************************************************************
** Constructor
*********************************************************************/
Medusa::Medusa() {
	name = "Medusa";
	armor = 3;
	totalStrength = 8;
	strengthLeft = 8;
	attDiceSides = 6;
	attDiceNum = 2;
	defDiceSides = 6;
	defDiceNum = 1;
}

/*********************************************************************
** Getname morphed to fullfill abstract base class requirement.
*********************************************************************/
string Medusa::getName()
{
	return "Medusa";
}

/*********************************************************************
** Attack overriden for chance to turn opponent to stone
*********************************************************************/
int Medusa::attack() {
	int attack = roll(attDiceSides, attDiceNum);
	// Turn opponent to stone, instantly killing them on a roll of 12
	if (attack == 12) {
		cout << "Medusa locks eyes with the opponent and turns them to stone." << endl;

		// Attack is set to always surpass an opponents strength, armor, and highest possible defense roll.
		attack = opponent->getStrengthLeft() + opponent->getArmor() + (opponent->getDefDiceNum() * opponent->getDefDiceSides());
	}

	// Otherwise attack as normal
	else {
		attackMessage(attack);
	}
	return attack;
}