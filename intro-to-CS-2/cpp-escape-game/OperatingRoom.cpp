/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: OperatingRoom implementation file.
**              Derived from space class. The operating room has one exit.
**              There is a refrigerator to open, and two items to collect.
**              Some options are only available when certain conditions are met.
*********************************************************************/
#include "OperatingRoom.hpp"

/*********************************************************************
** Prints a different description of room depending on current status
*********************************************************************/
void OperatingRoom::printDescription()
{
	if (status < 2) // Refrigerator is closed
	{
		cout << "You are in a small operating room. Ghastly looking surgeon's tools\n"
			"are arranged in trays near an operating table. Across the room is a refrigerator.\n\n";
	}

	else if (status > 1) // Refrigerator is open
	{
		cout << "You are in a small operating room. Ghastly looking surgeon's tools\n"
			"are arranged in trays near an operating table. Across the room, a refrigerator\n"
			"stands open. Inside, test tubes of dark red liquid line the shelves. Strangely,\n"
			"there are also several large, raw steaks in zip lock bags on the bottom shelf.\n"
			"At least, you hope they are steaks.\n\n";
	}
}

/*********************************************************************
** Loads options into the menuOptions vector based on spaces status
** Also checks inventory for required items
*********************************************************************/
void OperatingRoom::setMenuOptions(vector<string> &inventory)
{
	menuOptions.clear();

	menuOptions.push_back("1) Return to the laboratory");

	if (status == 0) // fridge is closed and saw is on table
	{
		menuOptions.push_back("2) Grab a saw from the tray of operating tools");
		menuOptions.push_back("3) Open the refrigerator");
	}

	else if (status == 1) // fridge is closed and saw is taken
	{
		menuOptions.push_back("2) Open the refrigerator");
	}

	else if (status == 2) // fridge is open and saw is on table.
	{
		menuOptions.push_back("2) Grab a saw from the tray of operating tools");
		menuOptions.push_back("3) Grab a steak from the fridge");
	}

	else if (status == 3) // fridge is open and saw is taken
	{
		menuOptions.push_back("2) Grab a steak from the fridge");
	}

	else if (status == 4) // fridge is open, steak is taken, saw is on table
	{
		menuOptions.push_back("2) Grab a saw from the tray of operating tools");
	}
}

/*********************************************************************
** Receives players menu selection and changes room status, prints description of player actions
** or status change, and adds items to player's inventory when applicable.
** Returns new location if player has exited space, otherwise returns this location.
*********************************************************************/
Space* OperatingRoom::changeStatus(int selection, vector<string> &inventory)
{
	// exit to laboratory
	if (selection == 1) 
	{
		return right;
	}

	// take the surgical saw
	else if ((status == 0 || status == 2 || status == 4) && selection == 2)
	{
		if (status == 0)
		{
			status = 1;
		}
		else
		{
			status = 3;
		}

		cout << "You place the surgical saw in your backpack.\n\n";

		inventory.push_back("Saw");
	}

	// Open the refrigerator
	else if ((status == 0 && selection == 3) || (status == 1 && selection == 2))
	{
		if (status == 0)
		{
			status = 2;
		}
		else
		{
			status = 3;
		}

		cout << "You open the refrigerator to find test tubes of dark red liquid lining the shelves.\n"
			"Strangely, there are also several large, raw steaks in zip lock bags on the bottom shelf.\n"
			"At least, you hope they are steaks.\n\n";
	}

	// Take steak from fridge
	else if ((status == 2 && selection == 3) || (status == 3 && selection == 2))
	{
		if (status == 2)
		{
			status = 4;
		}
		else
		{
			status = 5;
		}

		cout << "You put one of the bags containing steak in your backpack.\n\n";

		inventory.push_back("Steak");
	}
	
	// If location hasn't changed return this space as location
	return this;
}