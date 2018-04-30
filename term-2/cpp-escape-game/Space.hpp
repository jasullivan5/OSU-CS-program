/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Header file for the Space class.
**              Space is an abstract class that provides a framework
**              for classes that simulate a room or other space that
**              a player can interact with. It has up to four exits to
**              other spaces. It also has members and functions that
**              provide menu options and descriptions of what is in the
**              room and how these things can be interacted with by the
**              player.
*********************************************************************/
#ifndef SPACE_HPP
#define SPACE_HPP

#include<vector>
using std::vector;
#include<string>
using std::string;
#include <iostream>
using std::cout;
class Space
{
protected:

	// Pointers to other spaces
	Space* up;
	Space* right;
	Space* down;
	Space* left;

	bool gameOver;              // Indicates if a condition for the game to end has been met
	int status;                 // Space status alters various behaviors
	bool firstEntry;            // Flag indicates if player is entering space for first time
	vector<string> menuOptions; // Holds menu options for space
	vector<string> floorItems;  // Items on floor in space
	
public:
	Space();
	virtual ~Space();

	// Prints description of the space
	virtual void printDescription() = 0;

	// Sets menu options for player
	virtual void setMenuOptions(vector<string> &inventory) = 0; 

	// Status of room is changed based on player actions and current space status
	virtual Space* changeStatus(int selection, vector<string> &inventory) = 0;

	// Describes items on floor in space if any
	void printfloorItems(); 

	// Returns bool indicating if item was found in player's inventory
	bool searchInventory(vector<string> &inventory, string item);

	// Uses an item, removing it from player's inventory
	void useItem(vector<string> &inventory, string item);
	
	// Getter functions
	bool getGameOver();
	bool getFirstEntry();
	vector<string>* getMenuOptions();
	vector<string>* getFloorItems();
	
	// Setter functions
	void setUp(Space* ptr);
	void setRight(Space* ptr);
	void setDown(Space* ptr);
	void setLeft(Space* ptr);
	void setFirstEntry(bool fe);
	void setGameOver(bool go);
};
#endif