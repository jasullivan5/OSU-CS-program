#include "ScrewDriver.h"
#include "Spaceship.h"

ScrewDriver::ScrewDriver(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void ScrewDriver::use(Area* location, Player* player) {
	if(location->hasItem("Spaceship") && fixes > 0){
		//Use Power Crystal to fix spaceship
		Item* ship = location->getItem("Spaceship"); 
		if(ship !=NULL){
			cout << "You fixed all the loose screws on the spaceship, looks like it won't fall apart now.\n";
			((Spaceship*)ship)->fix();
			fixes--; 
		}
	}else{
		cout << "Looks like that didn't do anything.\n"; 
	}
}