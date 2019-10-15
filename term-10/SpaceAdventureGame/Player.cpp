#include "Player.h"
#include "Area.h"
#include "Item.h"

int Player::findItem(std::string targetName){
	for(int ii=0; ii < inventory.size(); ii++){
		if(inventory[ii]->getName().compare(targetName)==0){
			return ii; 	
		}
	}
	return -1; 
}

void Player::kill(){
	lifeRemaining = 0;
}


/*
void Player::initializeInventory(std::vector<Item*> addItem) {
	for(int i=0; i < addItem.size(); i++) {
		inventory.push_back(addItem[i]);
		std::cout << addItem[i]->getName() << std::endl;
	}
}
*/
void Player::addInventory(Item *item){
	inventory.push_back(item);
}
bool Player::drop(std::string targetName){
	for(int ii=0; ii < inventory.size(); ii++){
		if(inventory[ii]->getName().compare(targetName)==0){
			location->dropItem(inventory[ii]); 
			inventory.erase(inventory.begin()+ii); 
			return true; 	
		}
	}
	for(int ii=0; ii < clothesWorn.size(); ii++){
		if(clothesWorn[ii]->getName().compare(targetName)==0){
			location->dropItem(clothesWorn[ii]); 
			clothesWorn.erase(clothesWorn.begin()+ii); 
			return true; 	
		}
	}
	return false; 
} 

bool Player::take(std::string targetName){
	Item* targetPtr = location->takeItem(targetName); 
	if( targetPtr != NULL){
		if (targetPtr->getName().compare("Gas") == 0) {
			gasRemaining += 10;
		}
		else {
			inventory.push_back(targetPtr);
		}
		std::cout << "You picked up the " << targetPtr->getName() <<"\n"; 
		return true; 
	}else{
		return false;  
	}
}

Area* Player::getLocation(){
	return location; 
}
std::string Player::getLocationName(){
	return location->getName(); 
}

void Player::setVars(double gas, double life){
	lifeRemaining = life; 
	gasRemaining = gas; 
}
void Player::setLocation(Area* anArea){
	location = anArea; 
}
	 
void Player::listInventory(){
	std::cout << "Your inventory:\n"; 
	for(int ii=0; ii < inventory.size(); ii++){
		std::cout << inventory[ii]->getName() << "\n"; 
	}
	if(inventory.empty()){
		std::cout << "... is empty\n"; 
	}
	cout << "\n"; 
	std::cout << "You are wearing: \n"; 
	for(int ii=0; ii < clothesWorn.size(); ii++){
		std::cout << clothesWorn[ii]->getName() << "\n"; 
	}
	if(clothesWorn.empty()){
		std::cout << "Nothing... you are naked.\n"; 
	}
	cout << "\n"; 
}

bool Player::lookAt(std::string targetName){
	for(int ii=0; ii < inventory.size(); ii++){
		if(inventory[ii]->getName().compare(targetName)==0){
			inventory[ii]->lookAt(); 
			return true; 	
		}
	}
	for(int ii=0; ii < clothesWorn.size(); ii++){
		if(clothesWorn[ii]->getName().compare(targetName)==0){
			clothesWorn[ii]->lookAt(); 
			return true; 	
		}
	}
	return false; 
}

bool Player::use(std::string targetName){
	for(int ii=0; ii < inventory.size(); ii++){
		if(inventory[ii]->getName().compare(targetName)==0){
			Item* item = inventory[ii]; 
			if(item->isWearable()){
				wear(item); 
				inventory.erase(inventory.begin()+ii);
				cout << "You put on the " << targetName << ".\n"; 
				return true; 
			}else{
				inventory[ii]->use(location, this); 
				return true; 	
			}
		}
	}
	if (!targetName.empty()) {
		cout << "You don't have a " << targetName << " in your inventory.\n";
	}
	else {
		cout << "I'm not sure what item you are trying to use. Please specify one that is in your inventory.\n";
	}
	return false; 

}

bool Player::wear(Item* item){
	clothesWorn.push_back(item); 
	return true; 
}

bool Player::takeOff(std::string targetName){
	for(int ii=0; ii < clothesWorn.size(); ii++){
		if(clothesWorn[ii]->getName().compare(targetName)==0){
			inventory.push_back(clothesWorn[ii]); 
			clothesWorn.erase(clothesWorn.begin()+ii); 
			cout << "You took off the " << targetName << " and put it in your inventory.\n"; 
			return true; 
		}
	}
	return false; 
}

bool Player::eat(std::string targetName){
	int itemIndex = findItem(targetName); 
	if(itemIndex != -1){
		//Eat the item, add to asphyxiation timer?
		inventory[itemIndex]->use(location, this); 
		return true; 
	}else{
		return false; 
	}
}

bool Player::isWearing(std::string targetName){
	for(int ii=0; ii < clothesWorn.size(); ii++){
		if(clothesWorn[ii]->getName().compare(targetName)==0){
			return true; 	
		}
	}
	return false; 
}

bool Player::hasItem(std::string targetName){
	for(int ii=0; ii < inventory.size(); ii++){
		if(inventory[ii]->getName().compare(targetName)==0){
			return true; 	
		}
	}
	return false; 
}

bool Player::eraseItem(std::string targetName) {
	for (int ii = 0; ii < inventory.size(); ii++) {
		if (inventory[ii]->getName().compare(targetName) == 0) {
			inventory.erase(inventory.begin() + ii);
			return true;
		}
	}
	return false;
}

std::vector<Item*> Player::getPlayerInventory(){
	return inventory;
}
/***
bool Player::launch(){
	if(location->hasItem("Spaceship"){
		
	}
}
*/



