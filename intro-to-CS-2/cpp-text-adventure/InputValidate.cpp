/*********************************************************************
** Program name: Final Project
** Author: Joseph Sullivan
** Date: 6/12/2017
** Description: Implementation file for the InputValidation class.
**              This class allows the programmer to enter a string or
**              Menu object as a prompt to the user for input. It then
**              accepts user input and combines several flagging functions
**              to determine if the input meets certain criteria regarding
**              data type, minimum values, maximum values, and length.
**              The prompt/Menu is repeated until the user enters a valid
**              input. Once valid input is entered, it can be retrieved in
**              the desired data type with one of the get functions.
*********************************************************************/
#include "InputValidate.hpp"

/*********************************************************************
** Default constructor initializes fields.
*********************************************************************/
InputValidate::InputValidate() {
	validInput = true;
	min = 0;
	max = 0;
	strngMax = 0;
}

/*********************************************************************
** Gets a line of input from user as string.
*********************************************************************/
void InputValidate::setEntry() {
	getline(cin, entry);
}

/*********************************************************************
** Checks if input from user is empty. If so, it is flagged as invalid.
*********************************************************************/
void InputValidate::notEmpty() {
	if (entry.length() == 0) {
		validInput = false;
	}
}

/*********************************************************************
** Checks if string is longer than desired max length. If so, it is flagged as invalid.
*********************************************************************/
void InputValidate::strngMaxLngth() {
	if (entry.length() > strngMax) {
		validInput = false;
	}
}

/*********************************************************************
** Checks if input from user is only numbers. If not, it is flagged as invalid.
*********************************************************************/
void InputValidate::onlyNumbers() {
	for (int i = 0; i < entry.length(); ++i) {
		if (!isdigit(entry[i])) {
			validInput = false;
		}
	}
}

/*********************************************************************
** Determines if input is within chosen integer range. If not, it is flagged as invalid.
*********************************************************************/
void InputValidate::numInRange() {
	if (validInput == true) {
		int entryAsInt = getValidInt();
		if ((entryAsInt < min) || (entryAsInt > max)) {
			validInput = false;
		}
	}
}

/*********************************************************************
** Input validation for string prompts and integer inputs.
** Function prints a prompt for input, accepts input, and uses several
** flag functions to determine if entry is a single integer within a
** the defined range. If it is not, The user is alerted and the prompt
** is reprinted. This repeats until valid input is received.
*********************************************************************/
void InputValidate::validateInt(string prompt, int mn, int mx) {
	min = mn;
	max = mx;
	do {
		validInput = true;
		cout << prompt << flush;
		setEntry();
		notEmpty();
		onlyNumbers();
		numInRange();
		if (!validInput) {
			cout << endl << "Your input was invalid." << endl << endl;
		}
	} while (!validInput);
}

/*********************************************************************
** Input validation for Menu prompts and integer inputs.
** Function prints a Menu, accepts input, and uses several
** flag functions to determine if entry is a single integer within a
** the defined range. If it is not, The user is alerted and the prompt
** is reprinted. This repeats until valid input is received.
*********************************************************************/
void InputValidate::validateInt(Menu menu, int mn, int mx) {
	min = mn;
	max = mx;
	do {
		validInput = true;
		menu.printMenu();
		cout << flush;
		setEntry();
		notEmpty();
		onlyNumbers();
		numInRange();
		if (!validInput) {
			cout << endl << "Your input was invalid." << endl << endl;
		}
	} while (!validInput);
}

/*********************************************************************
** Input validation for string prompts and string inputs.
** Function prints a prompt for input, accepts input, and uses several
** flag functions to determine if entry is a non-empty string of length
** no greater than defined maximum length. If it is not, The user is
** alerted and the prompt is reprinted. This repeats until valid input is received.
*********************************************************************/
void InputValidate::validateString(string prompt, int strngMx) {
	strngMax = strngMx;
	do {
		validInput = true;
		cout << prompt << flush;
		setEntry();
		notEmpty();
		strngMaxLngth();
		if (!validInput) {
			cout << endl << "Your input was invalid." << endl << endl;
		}
	} while (!validInput);
}


/*********************************************************************
** Getter for the entry data member. Used to retrieve a string entry
** once it is determined to be valid.
*********************************************************************/
string InputValidate::getValidString() const {
	return entry;
}

/*********************************************************************
** Turns the user's string input into an integer. Only intended to be used
** if string has already been validated as containing only numbers.
** Also used to retrieve entry as integer once it is deemed valid.
*********************************************************************/
int InputValidate::getValidInt() const {
	int entryToInt = stoi(entry);
	return entryToInt;
}