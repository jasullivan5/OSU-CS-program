/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Car implementation file.
**              Derived from Space class. The car is the final space
**              in the game. The player can receive one of two endings
**              depending on whether or not they kept the knife.
**              The player can attempt to stab the wolf or search for
**              the car keys. Some options are only available when certain
**              conditions are met.
*********************************************************************/
#include "Car.hpp"

/*********************************************************************
** Prints a different description of room depending on current status
*********************************************************************/
void Car::printDescription()
{
	if (status == 0) // The creature hasn't been stabbed
	{
		cout << "The werewolf, its arms reaching through the broken window, is trying to pull\n"
			"you out of the car. If you could only reach your knife.\n\n";
	}

	else if (status == 1) // The creature has been stabbed
	{
		cout << "The beast has retreated momentarily. You must find the keys to the car.\n\n";
	}
}

/*********************************************************************
** Loads options into the menuOptions vector based on spaces status
** Also checks inventory for required items
*********************************************************************/
void Car::setMenuOptions(vector<string> &inventory)
{
	menuOptions.clear();

	menuOptions.push_back("1) Search for the car keys");

	if (status == 0) // creature hasn't been stabbed
	{
		menuOptions.push_back("2) Stab the creature with your knife");
	}
}

/*********************************************************************
** Receives players menu selection and changes room status, prints description of player actions
** or status change, and adds items to player's inventory when applicable.
** Returns new location if player has exited space, otherwise returns this location.
** Sets gameOver to true if an appropriate action is performed.
*********************************************************************/
Space* Car::changeStatus(int selection, vector<string> &inventory)
{
	if (status == 0 && selection == 1) // Search for keys before stabbing creature
	{
		cout << "It would be best for you to deal with the monster that is trying to eat you first.\n"
			"Priorities!\n\n";
	}

	else if (status == 0 && selection == 2) // try to stab the creature
	{
		// you have the knife
		if (searchInventory(inventory, "Pocket Knife"))
		{
			cout << "You plunge the knife into the back of the creatures hand. It retreats, letting\n"
				"out a howl of agony.\n\n";
			useItem(inventory, "Pocket Knife");
			status = 1;
		}

		// you don't have the knife YOU DIE!
		else
		{
			cout << "While fumbling for your knife, you recall that you left it somewhere in the mansion.\n"
				"The beast drags you from the vehicle and off into the nearby woods, never to be seen again.\n"
				"Whoops! You were eaten by a werewolf. Better luck next time!\n\n";

			// ends the game
			gameOver = true;
		}
	}

	else if (status == 1 && selection == 1) // search for keys after stabbing creature YOU WIN!
	{
		cout << "With the creature at bay, you flip down the visor, and a set of keys falls into\n"
			"your lap. Starting, the car you race down the driveway and out onto winding mountain roads.\n"
			"Congratulations! You have escaped the mansion!\n\n";

		// ends the game
		gameOver = true;
	}

	// If location hasn't changed return this space as location
	return this;
}