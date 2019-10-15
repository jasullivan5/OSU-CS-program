#include "Item.h"
#include <fstream>
/*
private: 
	std::string name; 
	std::string description; 
	bool breakable; 
	bool takeable; 
*/	

	
Item::Item(std::string nameIn, std::string descriptionIn, bool takeableIn){
	name = nameIn; 
	description = loadFile(descriptionIn); 
	takeable = takeableIn; 
}

std::string Item::loadFile(std::string fileName) {
	fileName = "./descriptorFiles/" + fileName;
	std::ifstream file(fileName.c_str());
	std::string str;
	getline(file, str);
	file.close();
	return str;
}

void Item::use(Area* location, Player* player){
	std::cout << "That didn't seem to do anything. \n"; 
}

void Item::lookAt(){
	std::cout << description << "\n"; 
} 

std::string Item::getName(){
	return name; 
}


void Item::setEdible(bool yummy){
	edible = yummy;
} 

bool Item::isTakeable(){
	return takeable; 
} 
