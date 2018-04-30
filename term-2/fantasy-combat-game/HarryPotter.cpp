/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Harry Potter class implementation file.
**              Harry Potter makes a miraculous recovery if defeated once. Strength is
**              restored to 20 after being reduced to or below 0 for the first time.
**              Attack: 2d6, Defense: 2d6, Armor 0, Strength 10.
*********************************************************************/

#include "HarryPotter.hpp"

/*********************************************************************
** Constructor
*********************************************************************/
HarryPotter::HarryPotter() {
	name = "Harry Potter";
	armor = 0;
	totalStrength = 10;
	strengthLeft = 10;
	attDiceSides = 6;
	attDiceNum = 2;
	defDiceSides = 6;
	defDiceNum = 2;
	recover = false;
}

/*********************************************************************
** Getname morphed to fullfill abstract base class requirement.
*********************************************************************/
string HarryPotter::getName()
{
	return "Harry Potter";
}

/*********************************************************************
** If Harry drops below 1 strength for the first time this combat, he recovers.
*********************************************************************/
void HarryPotter::defend(int attack) {
	// defense roll and message
	int defend = roll(defDiceSides, defDiceNum);
	int damage = attack - (defend + armor);
	if (damage > 0) {
		strengthLeft -= damage;
	}
	defendMessage(defend, damage);

	// if Harry hasn't recovered yet and strength is below 1,
	// Harry continues fighting and his strength is set to 20.
	if ((!recover) && (strengthLeft < 1)) {
		cout << "Harry goes down... but makes a miraculous recovery!\n";
		totalStrength = 20;
		strengthLeft = 20;
		recover = true;
		cout << "His strength is restored to " << strengthLeft << "." << endl;
	}
}