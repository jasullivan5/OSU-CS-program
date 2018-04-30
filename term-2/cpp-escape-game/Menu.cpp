/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Implementation file for the Menu class.
**              Stores title and list of options for a program menu.
**              Has functions for adding options, formatting menu width,
**              and printing the menu along with a prompt for user input.
*********************************************************************/
#include "Menu.hpp"
#include <string>

/*********************************************************************
** Constructors
*********************************************************************/
Menu::Menu() {
	size = 0;
	menuWidth = 0;
}

Menu::Menu(string t) {
	title = t;
	size = 0;
	menuWidth = title.length();
}

/*********************************************************************
** Setter for the menu title.
*********************************************************************/
void Menu::setTitle(string t) {
	title = t;
	menuWidth = title.length();
}

/*********************************************************************
** Adds a string to the list of options that menu will display.
*********************************************************************/
void Menu::addOption(string option) {
	options.push_back(option);
	size = options.size();
}

/*********************************************************************
** Adjusts width of menu if entry is longer than default width.
*********************************************************************/
void Menu::setMenuWidth() {
	for (int i = 0; i < size; ++i) {
		if (options[i].length() > menuWidth) {
			menuWidth = options[i].length();
		}
	}
}

/*********************************************************************
** Sets the menu width, prints the title centered above options, prints
** options, and prints prompt to user to select an option.
*********************************************************************/
void Menu::printMenu() {
	setMenuWidth();
	cout << setw((menuWidth - title.length()) / 2) << "" << title << endl;
	cout << setfill('*') << setw(menuWidth) << "" << endl;
	for (int i = 0; i < size; ++i) {
		cout << options[i] << endl;
	}
	cout << setfill('*') << setw(menuWidth) << "" << setfill(' ') << endl;
	cout << "Type a number from the menu above and press Enter: ";
}
