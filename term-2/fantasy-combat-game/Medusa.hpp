/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Medusa class header file.
**              Medusa can turn opponents to stone with her glare instantly 
**              defeating them when she rolls her maximum attack value.
**              Attack: 2d6, Defense: 1d6, Armor: 3, Strength: 8
*********************************************************************/
#ifndef MEDUSA_HPP
#define MEDUSA_HPP
#include "Creature.hpp"
class Medusa : public Creature
{
public:
	Medusa();
	string getName() override;
	int attack() override; // glare ability added
};
#endif
