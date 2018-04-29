/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Laboratory header file.
**              Derived from Space class. The laboratory has three exits.
**              The player can open the attic or enter the attic, operating
**              room, or reading room. Some options are only available when
**              certain conditions are met.
*********************************************************************/
#ifndef LABORATORY_HPP
#define LABORATORY_HPP

#include "Space.hpp"

class Laboratory :public Space
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