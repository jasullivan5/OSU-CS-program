/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Laboratory implementation file.
**              Derived from Space class. The laboratory has three exits.
**              The player can open the attic or enter the attic, operating
**              room, or reading room. Some options are only available when
**              certain conditions are met.
*********************************************************************/
#include "Laboratory.hpp"

/*********************************************************************
** Prints a different description of room depending on current status
*********************************************************************/
void Laboratory::printDescription()
{
	if (status == 0) // attic is closed
	{
		cout << "You are in a laboratory. The room is eerily lit by luminescent neon liquids\n"
			"that course and bubble through flasks, tubes, and other strange devices.\n"
			"To the west is a plain metal door. To the north you see a cord dangling from the\n"
			"ceiling.\n\n";
	}
	else if (status == 1) // attic is open
	{
		cout << "You are in a laboratory. The room is eerily lit by luminescent neon liquids\n"
			"that course and bubble through flasks, tubes, and other strange devices.\n"
			"To the west is a plain metal door. To the north, a ladder ascends to the attic.\n"
			"Heavy, ragged breathing can be heard coming from the opening in the ceiling.\n\n";
	}
}

/*********************************************************************
** Loads options into the menuOptions vector based on spaces status
** Also checks inventory for required items
*********************************************************************/
void Laboratory::setMenuOptions(vector<string> &inventory)
{
	menuOptions.clear();

	menuOptions.push_back("1) Exit lab through the metal door");

	if (status == 0) // attic is closed
	{
		menuOptions.push_back("2) Pull the dangling cord");
	}
	else if (status == 1) // attic is open
	{
		menuOptions.push_back("2) Climb the ladder to the attic");
	}

	menuOptions.push_back("3) Enter reading room through secret passage");
}

/*********************************************************************
** Receives players menu selection and changes room status, prints description of player actions
** or status change, and adds items to player's inventory when applicable.
** Returns new location if player has exited space, otherwise returns this location.
*********************************************************************/
Space* Laboratory::changeStatus(int selection, vector<string> &inventory)
{
	// Enter operating room
	if (selection == 1)
	{
		return left;
	}

	// Open the attic
	else if (status == 0 && selection == 2)
	{
		cout << "An attic door swings open, and a ladder drops to the floor.\n"
			"You hear heavy, ragged breathing coming from above.\n\n";

		status = 1;
	}

	// Enter the attic
	else if (status == 1 && selection == 2)
	{
		return up;
	}

	// Enter the reading room
	else if (selection == 3)
	{
		return down;
	}

	// If location hasn't changed return this space as location
	return this;
}