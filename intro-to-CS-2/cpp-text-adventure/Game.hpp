/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Game header file.
**              Game possesses an object of each space sub-class.
**              It sets up the pointer relationship between all of the spaces,
**              and tracks the player's inventory and location.
**              It also implements the games timer which allows 50 menu selections,
**              not counting inventory management choices, and adds additional menu
**              choices beyond space specific ones that allow inventory management
**              looking around, which reprints the rooms description and any items
**              on the floor, and exiting to the program menu. Game sets up a loop
**              to display a menu after each player action until a condition is met
**              that ends the game (timer runs out, or player has won/lost).
*********************************************************************/
#ifndef GAME_HPP
#define GAME_HPP

#include "ReadingRoom.hpp"
#include "Laboratory.hpp"
#include "OperatingRoom.hpp"
#include "Attic.hpp"
#include "BallRoom.hpp"
#include "Car.hpp"

class Game
{
private:
	Space* location;          // pointer to which space player is in
	vector<string> inventory; // inventory of items

	// Objects of the six space sub-classes
	ReadingRoom readingRoom;
	Laboratory laboratory;
	OperatingRoom operatingRoom;
	Attic attic;
	BallRoom ballRoom;
	Car car;
	
public:
	Game();
	void runGame();
	void dropItem();
	void getItem();
};
#endif

