/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: main function for Final Project.
**              Establishes main program menu and provides introduction
**              for beginning of game. Declares the game and calls runGame.
**              User can play game repeatedly until they choose to exit program.
*********************************************************************/
#include "Game.hpp"
#include "InputValidate.hpp"

int main()
{
	// Set up menu and input validation
	Menu menu1("GAME MENU");
	menu1.addOption("1) Start Game");
	menu1.addOption("2) Exit Program");
	InputValidate menu1Input;

	// Return to menu until player chooses to exit
	do
	{
		// Get main menu user input
		menu1Input.validateInt(menu1, 1, 2);
		cout << endl;

		// Give intro flavor text to begin game and explain rules and how to win
		if (menu1Input.getValidInt() == 1)
		{
			cout << "The game can be completed with a minimum of 22 actions.\n"
			"The player must grab the saw, steak, and rope along the way. The pocket knife\n"
			"must be in their inventory in the car to truly win the game, but it will still\n"
			"end without the pocket knife, only the player loses in that scenario.\n"
			"The timer is set to 50 turns, after which they player will automatically lose.\n"
			"The sequence of menu selections to win the game is: 1,2,2,1,2,2,2,2,1,2,2,2,3,2,1,3,3,3,2,1,2,1.\n\n";


			cout << "You are startled awake by a loud crack of thunder.\n"
			"Your head is pounding. Heavy rain can be heard pelting a nearby window.\n"
			"You can only make out faint shapes around you in the dark room.\n"
			"You try to stand from the chair you are seated in but realize\n"
			"that you are bound to it, your hands tied behind your back.\n"
			"Lightning briefly illuminates the room, and you spot your backpack\n"
			"in a chair. A lamp stands next to it. You manage to shuffle over\n"
			"to the chair, retrieve your pocket knife from the backpack's\n"
			"front pouch, and eventually cut through your bonds. Your memories\n"
			"are hazy. How did you get here? Who tied you up? But,\n"
			"most importantly, how do you escape?\n\n";

			// create game object and run game
			Game game;
			game.runGame();
		}
	} while (menu1Input.getValidInt() != 2);
	return 0;
}