/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Vampire class header file.
**              Vampire is able to charm enemies into not attacking 50% of the time.
**              Attack: 1d12, Defense: 1d6, Armor: 1, Strength: 18
*********************************************************************/
#ifndef VAMPIRE_HPP
#define VAMPIRE_HPP
#include "Creature.hpp"
class Vampire : public Creature
{
public:
	Vampire();
	string getName() override;
	void defend(int attack) override; // charm ability added.
};
#endif
