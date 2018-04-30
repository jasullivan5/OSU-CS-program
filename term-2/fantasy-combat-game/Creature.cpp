/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Creature class header file.
**              A class that provides a frame work for unique
**              creatures. All creatures will have the base
**              characteristics this class provides to be used in combat.
**              If a creature is hit by an attack, its armor deflects
**              an amount of damage equal to its value. Strength is a resource
**              that is reduced by the amount of the damage that a creature
**              takes. A creature's attack and defend functions will
**              return random integers produced within the function to
**              represent the accuracy of an attacker and ability of a
**              a defender to dodge an attack. The comparison of these
**              values is how a damage value is derived.
*********************************************************************/
#include "Creature.hpp"


/*********************************************************************
** Base class constructor.
*********************************************************************/
Creature::Creature() {
	armor = 0;
	totalStrength = 1;
	strengthLeft = 1;
	attDiceSides = 6;
	attDiceNum = 1;
	defDiceSides = 1;
	defDiceNum = 6;
	opponent = nullptr;
}

/*********************************************************************
** Base class destructor.
*********************************************************************/
Creature::~Creature() {
	
}

/*********************************************************************
** Setters and Getters
*********************************************************************/
void Creature::setOpponent(Creature* o) {
	opponent = o;
}
int Creature::getArmor() const {
	return armor;
}
int Creature::getStrengthLeft() const {
	return strengthLeft;
}
int Creature::getDefDiceSides() const {
	return defDiceSides;
}
int Creature::getDefDiceNum() const {
	return defDiceNum;
}

/*********************************************************************
** Simulates roll of a number of dice with specified number of sides.
*********************************************************************/
int Creature::roll(int sides, int dice) {
	int result = 0;

	for (int i = 0; i < dice; ++i) {
		result += rand() % sides + 1;
	}
	return result;
}

/*********************************************************************
** Prints creature type and result of attack roll.
*********************************************************************/
void Creature::attackMessage(int attack) const {
	cout << name << " attacks for " << attack << "." << endl;
}

/*********************************************************************
** Rolls an attack and prints the result
*********************************************************************/
int Creature::attack() {
	int attack = roll(attDiceSides, attDiceNum);
	attackMessage(attack);
	return attack;
}

/*********************************************************************
** Prints creature type, result of defend roll, and resulting damage.
*********************************************************************/
void Creature::defendMessage(int defend, int damage) const {
	// Message when damage is dealt
	if (damage > 0) {
		cout << name << " has " << armor << " armor and defends for " << defend
			<< ", reducing damage to " << damage << ".\n";
	}

	// Message when all damage in negated
	else if (damage < 1) {
		cout << name << " has " << armor << " armor and defends for " << defend
			<< ", negating all damage.\n";
	}

	// Print strength left compared to total strength
	cout << "Strength: " << strengthLeft << " / " << totalStrength << endl;
}

/*********************************************************************
** Roll defense and deduct resulting damage from strength, then print message
*********************************************************************/
void Creature::defend(int attack) {
	// roll defense and compare damage
	int defend = roll(defDiceSides, defDiceNum);
	int damage = attack - (defend + armor);

	// If damage surpasses defense + armor, deduct it from remaining strength
	if (damage > 0)	{
		strengthLeft -= damage;
	}

	// Print result of defense roll
	defendMessage(defend, damage);
}