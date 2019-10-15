#include "Doohickey.h"
#include "Spaceship.h"


Doohickey::Doohickey(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void Doohickey::use(Area* location, Player* player) {
	if(location->hasItem("Spaceship")){
		//Use Power Crystal to fix spaceship
		Item* ship = location->getItem("Spaceship"); 
		if(ship !=NULL){
			cout << "Not sure what this doohickey does but you found a spot it fits right into on the spaceship.\n";
			((Spaceship*)ship)->fix();
			player->eraseItem("Doohickey");
		}
	}else{
		cout << "Looks like that didn't do anything.\n"; 
	}
}


