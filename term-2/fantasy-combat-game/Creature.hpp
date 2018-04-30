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
#ifndef CREATURE_HPP
#define CREATURE_HPP
#include <string>
using std::string;
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
class Creature
{
protected:
	string name;       // A creatures type.
	int armor;         // Damage deflected if attack surpasses defense
	int totalStrength; // Total amount of damage a creature can take
	int strengthLeft;  // Strength left. Declines with damage
	int attDiceSides;  // Number of sides on creature's attack dice
	int attDiceNum;    // Number of dice a creature rolls for attack
	int defDiceSides;  // Number of sides on creature's defense dice
	int defDiceNum;    // Number of dice a creature rolls for defense
	Creature* opponent; // pointer to the creature's opponent creature
public:
	// Constructor
	Creature();

	// Destructor
	virtual ~Creature();

	// Setter functions
	void setOpponent(Creature* o);                

	// Getter functions
	virtual string getName() = 0; // Made pure virtual to make class abstract
	int getArmor() const;
	int getStrengthLeft() const;
	int getDefDiceSides() const;
	int getDefDiceNum() const;	

	// Helper function that sums and returns the roll of a specified 
	// number of dice with a specified number of sides.
	static int roll(int sides, int dice);

	// Prints the creature's type and result of its attack roll.
	void attackMessage(int attack) const;

	// Rolls attack & prints message. May be overriden for special abilities.
	virtual int attack();    
	
	// Prints the creature's type and result of its defend roll.
	void defendMessage(int defend, int damage) const;

	// Rolls defend & prints message. May be overriden for special abilities.
	virtual void defend(int attack);         
};
#endif