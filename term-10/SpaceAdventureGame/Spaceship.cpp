#include "Spaceship.h"

Spaceship::Spaceship(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void Spaceship::use(Area* location, Player* player) {
	cout << "Looks like that didn't do anything.\n"; 
}

bool Spaceship::isFixed(){
	if(fixesNeeded <= 0){
		return true; 
	} else {
		return false; 
	}
}


