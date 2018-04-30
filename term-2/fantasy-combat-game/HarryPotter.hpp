/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/5/2017
** Description: Harry Potter class header file.
**              Harry Potter makes a miraculous recovery if defeated once. Strength is
**              restored to 20 after being reduced to or below 0 for the first time.
**              Attack: 2d6, Defense: 2d6, Armor 0, Strength 10.
*********************************************************************/
#ifndef HARRY_POTTER_HPP
#define HARRY_POTTER_HPP
#include "Creature.hpp"
class HarryPotter : public Creature
{
private:
	bool recover; // Flag ensures that Harry only recovers once per combat
public:
	HarryPotter();
	string getName() override;
	void defend(int attack) override; // Recovery ability added
};
#endif
