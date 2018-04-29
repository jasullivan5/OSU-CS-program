/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Header file for the Menu class.
**              Stores title and list of options for a program menu.
**              Has functions for adding options, formatting menu width,
**              and printing the menu along with a prompt for user input.
*********************************************************************/
#ifndef MENU_HPP
#define MENU_HPP
#include<vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
using std::setfill;

using std::string;

class Menu
{
private:
	string title;           // Title to display above menu
	vector<string> options; // Vector holds each option of menu
	int size;               // Number of options in table
	int menuWidth;          // Width of menu used in print formatting

public:
	Menu();                        // Constructors
	Menu(string t);
	void setTitle(string t);       // Set the Menu title
	void addOption(string option); // Adds string argument to menu options
	void setMenuWidth();           // Sets menu width for print formatting
	void printMenu();              // Prints the menu
};
#endif

