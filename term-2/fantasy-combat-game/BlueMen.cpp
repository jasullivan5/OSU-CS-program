/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Blue Men class implementation file.
**              Mob of tiny blue men. Lose a defense die for every 4 points
**              of strength lost. Attack: 2d10, Defense: 3d6, Armor 3, Strength: 12
*********************************************************************/

#include "BlueMen.hpp"

/*********************************************************************
** Constructor
*********************************************************************/
BlueMen::BlueMen() {
	name = "Blue Men";
	armor = 3;
	totalStrength = 12;
	strengthLeft = 12;
	attDiceSides = 10;
	attDiceNum = 2;
	defDiceSides = 6;
	defDiceNum = 3;
}

/*********************************************************************
** Getname morphed to fullfill abstract base class requirement.
*********************************************************************/
string BlueMen::getName()
{
	return "Blue Men";
}

/*********************************************************************
** Blue Men lose a defense die for every 4 points of damage.
*********************************************************************/
void BlueMen::defend(int attack) {
	int oldDefDice = defDiceNum; // number of defense dice before damage

								 // roll defense as normal
	int defend = roll(defDiceSides, defDiceNum);
	int damage = attack - (defend + armor);
	if (damage > 0) {
		strengthLeft -= damage;
	}
	defendMessage(defend, damage);

	// Calculate number of defense dice
	if ((strengthLeft % 4) == 0) {
		defDiceNum = strengthLeft / 4;
	}
	else if ((strengthLeft % 4 != 0)) {
		defDiceNum = (strengthLeft / 4) + 1;
	}

	// If defense dice have changed, print message
	if (oldDefDice != defDiceNum) {
		cout << "Some Blue Men have perished.\n";
		cout << "Defense dice reduced from " << oldDefDice
			<< " to " << defDiceNum << "." << endl;
	}
}