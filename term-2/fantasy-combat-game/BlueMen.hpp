/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Blue Men class header file.
**              Mob of tiny blue men. Lose a defense die for every 4 points 
**              of strength lost. Attack: 2d10, Defense: 3d6, Armor 3, Strength: 12
*********************************************************************/
#ifndef BLUE_MEN_HPP
#define BLUE_MEN_HPP
#include "Creature.hpp"
class BlueMen : public Creature
{
public:
	BlueMen();
	string getName() override;
	void defend(int attack) override; // Loss of defense dice added
};
#endif

