#include "Mushroom.h"

Mushroom::Mushroom(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}


void Mushroom::use(Area* location, Player* player) {
	if(!location->hasOxygen()){
		location->setOxygen(true); 
		cout << "Wow you can suddenly breath.\n"; 
	}else{
		cout << "Looks like that didn't do anything.\n"; 
	}
	player->eraseItem("Mushroom");
}


