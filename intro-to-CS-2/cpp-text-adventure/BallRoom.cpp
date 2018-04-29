/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: BallRoom implementation file.
**              Derived from Space class. The ball room has two exits
**              and no items to obtain. Player can remove vines from door
**              if they have the saw with them, then they can exit through door.
*********************************************************************/
#include "BallRoom.hpp"

/*********************************************************************
** Prints a different description of room depending on current status
*********************************************************************/
void BallRoom::printDescription()
{
	if (status == 0) // Vines are on door
	{
		cout << "You are in a large ballroom. Thick woody vines covered in thorns grow along the\n"
			"floor, walls, and ceiling. Looking around the room, you see that they all grow from a\n"
			"twelve foot tall plant occupying a large terracotta pot in the far corner of the room.\n"
			"The plant appears to be topped by a massive flower bud. There is a door along the north wall,\n"
			"but it too is covered in vines that bar your exit. You would need something sharp to cut through them.\n\n";
	}

	if (status == 1) // Vines are removed from door
	{
		cout << "The great plant is dragging itself toward you with its vines. Its mouth is open wide,\n"
		"waiting to devour you. The only escape is through the door you have now cut free of vines.\n\n";
	}
}

/*********************************************************************
** Loads options into the menuOptions vector based on spaces status
** Also checks inventory for required items
*********************************************************************/
void BallRoom::setMenuOptions(vector<string> &inventory)
{
	menuOptions.clear();

	if (status == 0) // vines are on door
	{
		menuOptions.push_back("1) Climb the rope back to the reading room");

		// Action only available if player has saw
		if (searchInventory(inventory, "Saw"))
		{
			menuOptions.push_back("2) Use the surgical saw to cut the vines from the door");
		}
	}

	else if (status == 1) // vines are removed from door
	{
		menuOptions.push_back("1) Escape through the door");
	}
}

/*********************************************************************
** Receives players menu selection and changes room status, prints description of player actions
** or status change, and adds items to player's inventory when applicable.
** Returns new location if player has exited space, otherwise returns this location.
*********************************************************************/
Space* BallRoom::changeStatus(int selection, vector<string> &inventory)
{
	// climb back to reading room
	if (status == 0 && selection == 1)
	{
		return up;
	}

	// cut through vines with saw
	else if (status == 0 && selection == 2)
	{
		status = 1;
		
		cout << "As you begin sawing at the vines, you hear a rustling of leaves from behind you\n"
			"You peer over your shoulder to discover that what you thought was a flower bud, is actually\n"
			"the head of a giant carnivorous plant.The great plant is dragging itself toward you with its vines.\n"
			"Its mouth is open wide, waiting to devour you. The only escape is through the door you have now cut free of vines.\n\n";

		useItem(inventory, "Saw");
	}

	// exit ballroom through door
	else if (status == 1 && selection == 1)
	{
		cout << "You dash through the door and find yourself at the top of a grand staircase that\n"
			"appears to lead down to the main entrance of this mansion. Running down the stairs and out\n"
			"the door, you see a car parked a short distance away. The door is unlocked and you hop in.\n"
			"Just as you close the door, something crashes through the driver's side window.\n\n";

			return left;
	}

	// If location hasn't changed return this space as location
	return this;
}