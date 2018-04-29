/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Car header file.
**              Derived from Space class. The car is the final space
**              in the game. The player can receive one of two endings
**              depending on whether or not they kept the knife.
**              The player can attempt to stab the wolf or search for
**              the car keys. Some options are only available when certain
**              conditions are met.
*********************************************************************/
#ifndef CAR_HPP
#define CAR_HPP
#include "Space.hpp"
class Car :public Space
{
public:
	// Prints description of rooms current state
	void printDescription() override;

	// Determines actions available to player based on current state
	void setMenuOptions(vector<string> &inventory) override;

	// Changes status of room based on player actions and current state of room
	Space* changeStatus(int selection, vector<string> &inventory) override;
};
#endif