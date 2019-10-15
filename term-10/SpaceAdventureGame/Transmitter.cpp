#include "Transmitter.h"

Transmitter::Transmitter(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void Transmitter::use(Area* location, Player* player) {
	if (firstUse) {
		cout << "You rewire the transmitter to send a distress signal to Earth.\n";
		cout << "Unfortunately, this transmitter does not have an audio receiver... probably because Opportunity Rover doesn't have ears.\n";
		cout << "You have no way of knowing whether or not your transmission reached Earth.\n";
		firstUse = false;
	}
	else {
		cout << "You send another distress signal to Earth.\n";
	}
}
