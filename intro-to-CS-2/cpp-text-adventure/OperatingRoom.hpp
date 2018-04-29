/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: OperatingRoom header file.
**              Derived from space class. The operating room has one exit.
**              There is a refrigerator to open, and two items to collect.
**              Some options are only available when certain conditions are met.
*********************************************************************/
#ifndef OPERATING_ROOM_HPP
#define OPERATING_ROOM_HPP

#include "Space.hpp"

class OperatingRoom :public Space
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