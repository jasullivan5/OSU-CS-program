#include "Pickaxe.h"

Pickaxe::Pickaxe(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void Pickaxe::use(Area* location, Player* player) {
	if(location->hasItem("Crysallith")){
		//Make Crysallith takeable
		Item* item = location->getItem("Crysallith"); 
		if(item !=NULL){
			item->setTakeable(true);
			cout << "It appears you successfully mined the Crysallith.\n";
			player->take("Crysallith");
		}
	}else if(location->hasItem("PowerCrystal")){
		Item* item = location->getItem("PowerCrystal"); 
		if(item !=NULL){
			item->setTakeable(true); 
			cout << "It appears you uncovered a power crystal.\n";
			player->take("PowerCrystal");
		}
	}else{
		cout << "Looks like that didn't do anything.\n"; 
	}
}


