/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Attic header file.
**              Derived from space class. The attic has one exit and a
**              single item to be gathered. Player can turn on the lights
**              distract a monster, and grab a rope.
**              Some options are only available when certain conditions are met.
*********************************************************************/
#ifndef ATTIC_HPP
#define ATTIC_HPP

#include "Space.hpp"

class Attic :public Space
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
