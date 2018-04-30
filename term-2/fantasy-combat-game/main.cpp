/*********************************************************************
** Program name: Project 3
** Author: Joseph Sullivan
** Date: 5/7/2017
** Description: Fantasy Combat Game main function.
**              Game allows two players to choose fantasy creatures to
**              battle one another. Once the player's choose their creatures
**              from the menu, combat is simulated with dice rolls turn
**              by turn until one creature reduces the other creature's
**              strength points to zero or less. The surviving creature is
**              then declared the winner.
*********************************************************************/
#include "InputValidate.hpp"
#include "Vampire.hpp"
#include "Barbarian.hpp"
#include "BlueMen.hpp"
#include "Medusa.hpp"
#include "HarryPotter.hpp"
#include <ctime>

// Helper function to set each player's creature type.
void setPlayerCreature(Creature* &player, string num);

int main()
{
	
	srand(time(nullptr));        // seed random number function
	InputValidate mMenuInput;    // validation object for menu inputs
	
	// pointers for players' chosen creatures
	Creature* player1 = nullptr;
	Creature* player2 = nullptr;

	// Configure menu for game
	Menu mMenu("FANTASY COMBAT GAME");
	mMenu.addOption("1) Choose Combatants");
	mMenu.addOption("2) Quit Program");

	// Return to menu after combat until user selects Quit Program
	do {

		// Show main menu and get valid selection from user
		mMenuInput.validateInt(mMenu, 1, 2);
		cout << endl;

		// Option 1: user selects creatures and they battle
		if (mMenuInput.getValidInt() == 1) {

			// Display creature menu for each player and get selections
			setPlayerCreature(player1, "1");
			cout << endl;
			setPlayerCreature(player2, "2");
			cout << endl;

			// Set each creatures pointer to their opponent
			player1->setOpponent(player2);
			player2->setOpponent(player1);

			// Decide randomly who attacks first
			int turn = (rand() % 2 + 1);

			int round = 1;    // Round counter set to 1st round
			int newRound = 0; // Counter used to check if both players attacked

			// Begin combat
			cout << "ROUND 1" << endl;

			// Combat continues until a combatant has less than 1 strength
			while((player1->getStrengthLeft() > 0) && player2->getStrengthLeft() > 0) {
				
				// Counts when 2 attacks have occured and new round begins
				if (newRound == 2) {
					newRound = 0;
					round++;
					cout << "ROUND " << round << endl;
				}

				// Player one attacks and turn is passed to player two.
				if (turn == 1) {
					cout << "Player 1's Attack" << endl;
					player2->defend(player1->attack());
					turn = 2;
					cout << "Press Enter to Continue." << endl;
					cin.get();
				}

				// Or player two attack and turn is passed to player one.
				else if (turn == 2) {
					cout << "Player 2's Attack" << endl;
					player1->defend(player2->attack());
					turn = 1;
					cout << "Press Enter to Continue." << endl;
					cin.get();
				}

				// 1/2 of round is tallied as complete
				newRound++;
			}

			// Delcare victor if either players' strength is below 1.
			if (player1->getStrengthLeft() < 1) {
				cout << "Player 2's " << player2->getName() << " has won the battle!\n" << endl;
			}
			else if (player2->getStrengthLeft() < 1) {
				cout << "Player 1's " << player1->getName() << " has won the battle!\n" << endl;
			}
		}
	} while (mMenuInput.getValidInt() != 2);

	// Free memory used for creatures
	delete player1;
	delete player2;

	return 0;
}

/*********************************************************************
** Helper function displays creature selection menu for a player
*********************************************************************/
void setPlayerCreature(Creature* &player, string num) {

	// Set up menu title, options, validation, and get input
	string title = "CHOOSE A COMBATANT PLAYER " + num;
	InputValidate mMenuSub1Input;
	Menu mMenuSub1(title);
	mMenuSub1.addOption("1) Vampire");
	mMenuSub1.addOption("2) Barbarian");
	mMenuSub1.addOption("3) Blue Men");
	mMenuSub1.addOption("4) Medusa");
	mMenuSub1.addOption("5) Harry Potter");
	mMenuSub1Input.validateInt(mMenuSub1, 1, 5);

	// Assign creature to player based on their selection
	if (mMenuSub1Input.getValidInt() == 1) {
		player = new Vampire;
	}
	else if (mMenuSub1Input.getValidInt() == 2) {
		player = new Barbarian;
	}
	else if (mMenuSub1Input.getValidInt() == 3)	{
		player = new BlueMen;
	}
	else if (mMenuSub1Input.getValidInt() == 4)	{
		player = new Medusa;
	}
	else if (mMenuSub1Input.getValidInt() == 5)	{
		player = new HarryPotter;
	}
}