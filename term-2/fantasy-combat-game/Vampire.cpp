/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Vampire class impelementation file.
**              Vampire is able to charm enemies into not attacking 50% of the time.
**              Attack: 1d12, Defense: 1d6, Armor: 1, Strength: 18
*********************************************************************/

#include "Vampire.hpp"

/*********************************************************************
** Constructor
*********************************************************************/
Vampire::Vampire() {
	name = "Vampire";
	armor = 1;
	totalStrength = 18;
	strengthLeft = 18;
	attDiceSides = 12;
	attDiceNum = 1;
	defDiceSides = 6;
	defDiceNum = 1;
}

/*********************************************************************
** Getname morphed to fullfill abstract base class requirement.
*********************************************************************/
string Vampire::getName()
{
	return "Vampire";
}

/*********************************************************************
** Defend roll overriden to include Vampire's chance to charm opponent
*********************************************************************/
void Vampire::defend(int attack) {
	// roll 50% chance to charm opponent into not attacking
	int charm = roll(2, 1);

	// take no damage if opponent is charmed
	if (charm == 1) {
		cout << "Vampire charmed opponent into not attacking." << endl;
	}

	// otherwise roll defense as normal
	else if (charm == 2) {
		int defend = roll(defDiceSides, defDiceNum);
		int damage = attack - (defend + armor);
		if (damage > 0) {
			strengthLeft -= damage;
		}
		defendMessage(defend, damage);
	}
}
