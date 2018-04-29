/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Attic implementation file.
**              Derived from space class. The attic has one exit and a
**              single item to be gathered. Player can turn on the lights
**              distract a monster, and grab a rope.
**              Some options are only available when certain conditions are met.
*********************************************************************/
#include "Attic.hpp"

/*********************************************************************
** Prints a different description of room depending on current status
*********************************************************************/
void Attic::printDescription()
{
	if (status == 0) // Lights are off
	{
		cout << "You are in the attic. A pair of glowing red eyes watch you from\n"
			"across the dark room. The faint light from below reveals a light bulb with a\n"
			"a pull cord just above you.\n\n";
	}

	else if (status == 1) // Lights are on
	{
		cout << "The beast pulls at its chain. It gnashes its fangs and swipes at you\n"
			"with fearsome claws, but you remain just out of reach. Passed the beast, you\n"
			"see a rope hanging from a hook on the wall. A dog bowl lies on the floor next to a pile\n"
			"of bones.\n\n";
	}

	else if (status == 2) // Steak has been thrown
	{
		cout << "The beast is distracted. It tears at the hunk of meat in a frenzy.\n"
			"If you hurry, you may be able to sneak by and grab the rope.\n\n";
	}

	else if (status == 3) // Rope has been taken
	{
		cout << "Its hunger unsated, the beast lunges for you again. You see the\n"
			"chain beginning to come free from the wall as the beast pulls against it.\n\n";
	}
}

/*********************************************************************
** Loads options into the menuOptions vector based on spaces status
** Also checks inventory for required items
*********************************************************************/
void Attic::setMenuOptions(vector<string> &inventory)
{
	menuOptions.clear();

	menuOptions.push_back("1) Return to the laboratory");

	if (status == 0) // light is off
	{
		menuOptions.push_back("2) Turn on the light");
	}

	else if (status == 1) // light is on
	{
		menuOptions.push_back("2) Go for the rope");

		// Option only available if player has Steak in inventory
		if (searchInventory(inventory, "Steak"))
		{
			menuOptions.push_back("3) Throw the steak into the beast's bowl");
		}
	}

	else if (status == 2) // Steak is thrown, creature is distracted
	{
		menuOptions.push_back("2) Go for the rope");
	}
}

/*********************************************************************
** Receives players menu selection and changes room status, prints description of player actions
** or status change, and adds items to player's inventory when applicable.
** Returns new location if player has exited space, otherwise returns this location.
*********************************************************************/
Space* Attic::changeStatus(int selection, vector<string> &inventory)
{
	// exit to laboratory
	if (selection == 1)
	{
		return down;
	}

	// turn on lights
	else if (status == 0 && selection == 2)
	{
		status = 1;

		cout << "As you reach for the light, the glowing red eyes rush at you in the dark.\n"
			"The light flashes on just as a large fanged maw snaps shut inches from your face.\n"
			"A beast, part man, part wolf, stands in front of you. It is chained to the far wall.\n\n";

		printDescription();
	}

	// Attempt to go for rope before creature is distracted
	else if (status == 1 && selection == 2)
	{
		cout << "You would never make it passed the beast. You need a distraction of some kind.\n\n";
	}

	// distract creature with steak
	else if (status == 1 && selection == 3)
	{
		status = 2;

		cout << "As you pull out the raw steak, the beasts rage subsides. It sits on its haunches and\n"
			"wags its tail, obediently waiting for you to toss the steak. You throw it into the bowl, and the\n"
			"the creature pounces on it immediately. It is distracted for now.\n\n";

		useItem(inventory, "Steak");
	}

	// Get the rope while creature is distracted
	else if (status == 2 && selection == 2)
	{
		status = 3;

		cout << "You creep by the monster as it feasts, grab the rope, and hurry back to the ladder.\n\n";

		inventory.push_back("Rope");
	}

	// If location hasn't changed return this space as location
	return this;
}