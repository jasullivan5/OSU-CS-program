/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Implementation file for the Space class.
**              Space is an abstract class that provides a framework
**              for classes that simulate a room or other space that
**              a player can interact with. It has up to four exits to
**              other spaces. It also has members and functions that
**              provide menu options and descriptions of what is in the
**              room and how these things can be interacted with by the
**              player.
*********************************************************************/
#include "Space.hpp"

// Constructor
Space::Space()
{
	up = nullptr;
	right = nullptr;
	down = nullptr;
	left = nullptr;
	gameOver = false;
	status = 0;
	firstEntry = true;
}

// Destructor
Space::~Space()
{
	// implemented because it is virtual
	// I'm not sure if there is a way to make destructors virtual without writing implementation.
	// I always seem to receive errors if I don't include this blank implementation.
}

/*********************************************************************
** Prints items that the player has dropped on the floor.
*********************************************************************/
void Space::printfloorItems()
{
	cout << "The ";
	if (floorItems.size() == 1)
	{
		cout << floorItems[0] + " is on the floor.\n\n";
	}
	else if (floorItems.size() == 2)
	{
		cout << floorItems[0] + " and " + floorItems[1] + " are on the floor.\n\n";
	}
	else
	{
		for (int i = 0; i < floorItems.size(); ++i)
		{
			if (i != (floorItems.size() - 1))
			{
				cout << floorItems[i] + ", ";
			}
			else
			{
				cout << "and " + floorItems[i] + " are on the floor.\n\n";
			}
		}
	}
}

/*********************************************************************
** Returns bool indication if the item passed to function is found in the inventory
** passed to function.
*********************************************************************/
bool Space::searchInventory(vector<string> &inventory, string item)
{
	bool itemFound = false;
	for (int i = 0; i < inventory.size(); ++i)
	{
		if (inventory[i] == item)
		{
			itemFound = true;
		}
	}
	return itemFound;
}

/*********************************************************************
** If item is found in inventory, it is used and removed from inventory
*********************************************************************/
void Space::useItem(vector<string> &inventory, string item)
{
	for (int i = 0; i < inventory.size(); ++i)
	{
		if (inventory[i] == item)
		{
			inventory.erase(inventory.begin() + i);
		}
	}
}


/*********************************************************************
** Getter functions
*********************************************************************/
bool Space::getGameOver()
{
	return gameOver;
}
bool Space::getFirstEntry()
{
	return firstEntry;
}
vector<string>* Space::getMenuOptions()
{
	vector<string>* MOPtr = &menuOptions;
	return MOPtr;
}
vector<string>* Space::getFloorItems()
{
	vector<string>* FIPtr = &floorItems;
	return FIPtr;
}

/*********************************************************************
** Setter functions
*********************************************************************/
void Space::setUp(Space* ptr)
{
	up = ptr;
}
void Space::setRight(Space* ptr)
{
	right = ptr;
}
void Space::setDown(Space* ptr)
{
	down = ptr;
}
void Space::setLeft(Space* ptr)
{
	left = ptr;
}
void Space::setFirstEntry(bool fe)
{
	firstEntry = fe;
}
void Space::setGameOver(bool go)
{
	gameOver = go;
}