/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Barbarian class header file.
**              Barbarian Attack: 2d6, Defense: 2d6, Armor: 0, Strength: 12
*********************************************************************/
#ifndef BARBARIAN_HPP
#define BARBARIAN_HPP
#include "Creature.hpp"
class Barbarian : public Creature
{
public:
	Barbarian();
	string getName() override;
};
#endif
