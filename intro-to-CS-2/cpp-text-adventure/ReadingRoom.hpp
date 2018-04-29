/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: ReadingRoom header file.
**              Derived from Space class. ReadingRoom is the starting
**              room for the game. It has two exits but no items
**              to be obtained. The player can turn the lights on, open
**              a secret passage, and exit through the window or secret
**              passage. Some options are only available when the player
**              possesses certain items or certain conditions have been met.
*********************************************************************/
#ifndef READING_ROOM_HPP
#define READING_ROOM_HPP

#include "Space.hpp"

class ReadingRoom :public Space
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