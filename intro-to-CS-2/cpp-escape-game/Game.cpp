/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Game implementation file.
**              Game possesses an object of each space sub-class.
**              It sets up the pointer relationship between all of the spaces,
**              and tracks the player's inventory and location.
**              It also implements the games timer which allows 50 menu selections,
**              and adds additional menu choices beyond space specific ones that allow 
**              inventory management looking around, which reprints the rooms description 
**              and any items on the floor, and exiting to the program menu. Game sets up
**              a loop to display a menu after each player action until a condition is met
**              that ends the game (timer runs out, or player has won/lost).
*********************************************************************/
#include "Game.hpp"
#include "InputValidate.hpp"
using std::to_string;

/*********************************************************************
** Constructor
*********************************************************************/
Game::Game()
{
	// Set connections between rooms
	readingRoom.setUp(&laboratory);
	readingRoom.setDown(&ballRoom);
	laboratory.setDown(&readingRoom);
	laboratory.setLeft(&operatingRoom);
	laboratory.setUp(&attic);
	operatingRoom.setRight(&laboratory);
	attic.setDown(&laboratory);
	ballRoom.setUp(&readingRoom);
	ballRoom.setLeft(&car);
	car.setRight(&ballRoom);

	// Player starts in reading room
	location = &readingRoom;    
	
	// set false to prevent description from printing
	location->setFirstEntry(false); 
	
	// Inventory starts out with knife and book in it
	inventory.push_back("Pocket Knife");
	inventory.push_back("C++ Book");
}

/*********************************************************************
** A menu of actions for the player to choose is developed from unique location options based
** on a locations status and then some standard actions that can be performed in any location
** are added. Player chooses an option and result of action is displayed, potentially altering
** location and/or inventory. This repeats until timer expires, player chooses to exit to program
** menu, or condition is met that ends the game.
*********************************************************************/
void Game::runGame()
{
	int timer = 0;           // Timer limits number of turns before game ends automatically
	InputValidate menuInput; // Input validation object for menu
	int count;               // count used in while loop to get menu options from location

	// Return to actions menu until player exits or game ends
	do{

		// define menu object and title
		Menu menu("ACTION MENU");

		// Call location to set menu options determined by location's status
		location->setMenuOptions(inventory);

		// Add menu options from location to action menu
		count = 0;
		while(count < location->getMenuOptions()->size())
		{
			menu.addOption(location->getMenuOptions()->at(count));
			++count;
		}

		// After unique location actions are added, add standard actions that can be performed
		// in any space
		menu.addOption(to_string(count + 1) + ") Look around");
		menu.addOption(to_string(count + 2) + ") Drop an Item");
		menu.addOption(to_string(count + 3) + ") Pick up an Item");
		menu.addOption(to_string(count + 4) + ") Exit to program menu");

		// If it is player's first time in location, print its description and set firstEntry false
		if (location->getFirstEntry())
		{
			location->printDescription();
			location->setFirstEntry(false);
		}

		// print menu and get validated input from user
		menuInput.validateInt(menu, 1, count + 4);
		cout << endl;

		// If player's selection is unique to location, change status of location
		if (menuInput.getValidInt() <= count)
		{
			location = location->changeStatus(menuInput.getValidInt(), inventory);

			// If action causes player to exceed inventory size of 3, force player to drop an item
			while(inventory.size() > 3)
			{
				cout << "You have too many items. You must drop something.\n\n";
				dropItem();
			}
		}

		// print room description and any items on floor if player chooses to look around
		else if(menuInput.getValidInt() == count + 1)
		{
			location->printDescription();
			if (!location->getFloorItems()->empty())
			{
				location->printfloorItems();
			}
		}

		// player chooses to drop an item. call dropItem function
		else if(menuInput.getValidInt() == count + 2)
		{
			dropItem();
		}

		// player chooses to pick up an item. Allow if inventory has fewer than 3 items
		else if (menuInput.getValidInt() == count + 3)
		{
			if (inventory.size() == 3)
			{
				cout << "Your backpack is full. You must drop an item before picking any up.\n\n";
			}
			else
			{
				getItem();
			}
		}

		timer++; // increment timer

		// If player has cycled through menu 50 times, game ends
		if (timer >= 50)
		{
			cout << "A man in a white lab coat appears on the scene and fires a gun at you.\n"
				"You are hit with a dart. As you fade from consciousness, you hear the man say,\n"
				"\"Thought you could escape did you. Why, I haven't even began my experiments yet.\"\n"
				"You here him laugh maniacally as all fades to black, then silence.\n\n"
				"You did not escape the mansion in time. Please try again.\n\n";

			location->setGameOver(true);
		}

	// Play continues unless player chooses to exit or condition is met to end game
	} while ((menuInput.getValidInt() != count + 4) && location->getGameOver() == false);
}

/*********************************************************************
** If player has items, they can select one to drop on the floor at their location
*********************************************************************/
void Game::dropItem()
{
	if (inventory.size() > 0)
	{
		// Set up input validation and menu title
		InputValidate menuInput;
		Menu menu("BACKPACK: Select an item to drop it");

		// Count and loop number the items and add them as menu options
		int count = 0;
		while (count < inventory.size())
		{
			menu.addOption(to_string(count + 1) + ") " + inventory[count]);
			++count;
		}

		// Player may cancel, not dropping any items
		menu.addOption(to_string(count + 1) + ") Cancel");

		// validate player's entry
		menuInput.validateInt(menu, 1, count + 1);

		// If player hasn't canceled, remove item from inventory and add to locations floorItems vector
		if (menuInput.getValidInt() < count + 1)
		{
			string tempItem = inventory[menuInput.getValidInt() -1];
			inventory.erase(inventory.begin() + (menuInput.getValidInt() - 1));
			location->getFloorItems()->push_back(tempItem);
		}
	}

	// alert player if they have no items to drop
	else
	{
		cout << "Your backpack is empty." << endl;
	}
	cout << endl;
}

/*********************************************************************
** Player may pick up an item on the floor at location if there are any
*********************************************************************/
void Game::getItem()
{
	// if there are items on the floor
	if (location->getFloorItems()->size() > 0)
	{
		// set input validation and menu title
		InputValidate menuInput;
		Menu menu("ITEMS ON FLOOR: Select an item to pick it up");

		// Count and loop number the items and add them as menu options
		int count = 0;
		while (count < location->getFloorItems()->size())
		{
			menu.addOption(to_string(count + 1) + ") " + location->getFloorItems()->at(count));
			++count;
		}

		// option to  cancel is also added
		menu.addOption(to_string(count + 1) + ") Cancel");

		// validate user input
		menuInput.validateInt(menu, 1, count + 1);

		// If player doesn't cancel, remove item from floor and place in inventory
		if (menuInput.getValidInt() < count + 1)
		{
			string tempItem = location->getFloorItems()->at(menuInput.getValidInt() - 1);
			location->getFloorItems()->erase(location->getFloorItems()->begin() + (menuInput.getValidInt() - 1));
			inventory.push_back(tempItem);
		}
	}

	// Alert user if there are no items to pick up
	else
	{
		cout << "There are no items on the floor to pick up." << endl;
	}
	cout << endl;
}




