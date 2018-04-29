/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Header file for the InputValidation class.
**              This class allows the programmer to enter a string or
**              Menu object as a prompt to the user for input. It then
**              accepts user input and combines several flagging functions
**              to determine if the input meets certain criteria regarding
**              data type, minimum values, maximum values, and length.
**              The prompt/Menu is repeated until the user enters a valid
**              input. Once valid input is entered, can retrieved in
**              the desired data type with one of the get functions.
*********************************************************************/
#ifndef INPUT_VALIDATION_HPP
#define INPUT_VALIDATION_HPP
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::flush;
#include <string>
using std::string;
using std::stoi;
#include "Menu.hpp"

class InputValidate
{
private:
	string entry;    // holds the user's most recent entry.
	bool validInput; // flag to indicate if input is valid or not.
	int min, max;    // used when inputs must be within a given range.
	int strngMax;    // used when strings need a maximum length.

public:
	InputValidate();                  // Default constructor initializes fields.
	void setEntry();                  // Gets line of input from user and stores as entry.
	void notEmpty();                  // Flags empty string entries invalid.
	void strngMaxLngth();             // Flags strings long than max length as invalid.
	void onlyNumbers();               // Flags entries containing non-digit characters invalid.
	void numInRange();                // Flags values outside of min - max range invalid.

									  // Combines flags to validate an integer within a range.
	void validateInt(string prompt, int mn, int mx);

	// Overload to accept menu as prompt.
	void validateInt(Menu menu, int mn, int mx);

	// Combines flags to validate a string with maximum length.
	void validateString(string prompt, int strngMx);

	// Getter functions
	int getValidInt() const;
	string getValidString() const;
};
#endif
