/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: BallRoom header file.
**              Derived from Space class. The ball room has two exits
**              and no items to obtain. Player can remove vines from door
**              if they have the saw with them, then they can exit through door.
*********************************************************************/
#ifndef BALL_ROOM_HPP
#define BALL_ROOM_HPP

#include "Space.hpp"

class BallRoom :public Space
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