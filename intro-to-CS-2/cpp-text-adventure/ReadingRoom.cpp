/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: ReadingRoom implementation file.
**              Derived from Space class. ReadingRoom is the starting
**              room for the game. It has two exits but no items
**              to be obtained. The player can turn the lights on, open
**              a secret passage, and exit through the window or secret
**              passage. Some options are only available when the player
**              possesses certain items or certain conditions have been met.
*********************************************************************/
#include "ReadingRoom.hpp"

/*********************************************************************
** Prints a different description of room depending on current status
*********************************************************************/
void ReadingRoom::printDescription()
{
	if (status == 0) // Lights are off
	{
		cout << "It is dark. Rain pounds on a window to the east. As lightning flashes,\n"
			"you could swear that you see a pair of eyes watching you from the far west\n"
			"end of the room. You remember seeing a lamp near the chair that held your backpack.\n\n";
	}
	else if (status == 1) // Lights are on
	{
		cout << "You are in a reading room. The east wall is lined with book shelves\n"
			"on either side of a large window. Outside, a violent storm rages.\n"
			"To the north is a large stone fireplace that is unlit. At the far west\n"
			"end of the room, The head of a great stag is mounted on the wall. The door\n"
			"on the south wall is closed.\n\n";
	}
	else if (status == 2) // Lights are on and secret door is open
	{
		cout << "You are in a reading room. Shelves of books line the east wall\n"
			"on either side of a large window. Outside, a violent storm rages.\n"
			"To the north a passage is open at the back of a large stone fireplace. At the far west\n"
			"end of the room, The head of a great stag is mounted on the wall, one antler\n"
			"twisted downward. The door on the south wall is closed.\n\n";
	}
	else if (status == 3) // Lights are on, secret door is open, and rope provides exit out window
	{
		cout << "You are in a reading room. Shelves of books line the east wall\n"
			"on either side of an open window from which a rope descends to a balcony far below.\n"
			"To the north a passage is open at the back of a large stone fireplace. At the far west\n"
			"end of the room, The head of a great stag is mounted on the wall, one antler\n"
			"twisted downward. The door on the south wall is closed.\n\n";
	}
}

/*********************************************************************
** Loads options into the menuOptions vector based on spaces status
** Also checks inventory for required items
*********************************************************************/
void ReadingRoom::setMenuOptions(vector<string> &inventory)
{
	menuOptions.clear();

	if (status == 0) // lights off
	{
		menuOptions.push_back("1) Turn on the lamp");
	}
	else if (status == 1) // lights on
	{
		menuOptions.push_back("1) Try opening the south door");
		menuOptions.push_back("2) Examine mounted stag's head");
	}

	else if (status == 2) // lights on, passage open
	{
		menuOptions.push_back("1) Try opening the south door");
		menuOptions.push_back("2) Enter passage at back of fireplace");

		// only allow window exit to open if player has rope
		if (searchInventory(inventory, "Rope"))
		{
			menuOptions.push_back("3) Lower rope from window");
		}
	}

	else if (status == 3) // lights on, passage open, window exit open
	{
		menuOptions.push_back("1) Try opening the south door");
		menuOptions.push_back("2) Enter passage at back of fireplace");
		menuOptions.push_back("3) Climb out window and down rope");
	}
}

/*********************************************************************
** Receives players menu selection and changes room status, prints description of player actions
** or status change, and adds items to player's inventory when applicable.
** Returns new location if player has exited space, otherwise returns this location.
*********************************************************************/
Space* ReadingRoom::changeStatus(int selection, vector<string> &inventory)
{
	// Turn on lamp
	if (status == 0 && selection == 1)
	{
		status = 1;

		cout << "You turn on the lamp and are finally able to take in your\n"
			"surroundings.\n\n";

		printDescription();
	}

	// Try to open south door
	else if (status != 0 && selection == 1)
	{
		cout << "The door is locked. Someone obviously didn't want you leaving this room.\n\n";
	}

	// Flip switch that opens secret passage
	else if (status == 1 && selection == 2)
	{
		status = 2;

		cout << "You stand on a chair to have a closer look at the stag's head.\n"
			"Feeling around, you notice that one of its giant antlers is somehow loose.\n"
			"You give it a pull. It rotates forward and down with a click.\n"
			"The sound of grinding stone can be heard coming from the fireplace.\n"
			"Returning to the center of the room, you see that a passage has opened.\n\n";
	}

	// Enter secret passage
	else if (status > 1 && selection == 2)
	{
		return up;
	}

	// Use rope to open window exit
	else if (status == 2 && selection == 3)
	{
		useItem(inventory, "Rope");

		status = 3;

		cout << "You tie the rope around the south door's handle and send its length out\n"
			"the window to dangle above a large stone balcony far below.\n\n";
	}

	// Exit out window
	else if (status == 3 && selection == 3)
	{
		cout << "You cautiously climb several stories down as the unrelenting storm quickly\n"
			"soaks you to the bone. You touch down on the balcony and hurriedly enter the\n"
			"mansion again through a windowed double door.\n\n";

		return down;
	}

	// If location hasn't changed return this space as location
	return this;
}
