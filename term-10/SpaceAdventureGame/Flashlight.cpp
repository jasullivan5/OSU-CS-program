#include "Flashlight.h"

Flashlight::Flashlight(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void Flashlight::use(Area* location, Player* player) {
	if(location->isDark()){
		cout << "You flip on the flashlight and take a look around.\n"; 
		location->setDark(false); 
		location->look(); 
	} else{
		cout << "You flip on the flashlight but it's not too dark here so everything looks the same...\n"; 
	}
}


