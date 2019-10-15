#include <iostream>
#include <fstream>
#include "Area.h"
#include "Item.h"
#include "Exit.h"
/*
private:
	string areaName;
	string longDescription;
	string shortDescription;
	vector<Exit*> exits;
	vector<Item*> items;
	vector<Action> availableActions;
	bool firstEntry;
*/

Area::Area(std::string name, std::string shortFileName, std::string longFileName, std::string interFile, bool isDark, bool hasOxygen, bool isSpace){
	areaName=name;
	dark=isDark;
	oxygen=hasOxygen;
	space=isSpace;
	uIDisplay=interFile;
	//to do: update open file for reading in main
	shortDescription=loadFile(shortFileName);
	longDescription=loadFile(longFileName);
	uIDisplay=loadFile(interFile);

	longFileNames=longFileName;
	shortFileNames=shortFileName;
	uiFileName=interFile;
}

std::string Area::loadFile(std::string fileName) {
	//fileName = "./descriptorFiles/" + fileName;
	std::ifstream file(fileName.c_str());
	std::string str;
	getline(file, str);
	file.close();
	return str;
}

void Area::printDescription(){

	if(firstEntry){
		std::cout << longDescription << "\n";
		firstEntry = false;
		if (exits.size() > 0) {
			std::cout << "There are some possible exits here:\n";
			for (int ii = 0; ii < exits.size(); ii++) {
				exits[ii]->printName();
				if (this->getName().compare("Space") == 0) {
					cout << " - Gas needed: " << exits[ii]->getGasDistance() << ", Travel time: " << exits[ii]->getLifeDistance() << " years.";
				}
				std::cout << "\n";
			}
		}
		cout << "\n"; 
	} else {
		std::cout << shortDescription << "\n";
		if (exits.size() > 0) {
			std::cout << "There are some possible exits here:\n";
			for (int ii = 0; ii < exits.size(); ii++) {
				exits[ii]->printName();
				if (this->getName().compare("Space") == 0) {
					cout << " - Gas needed: " << exits[ii]->getGasDistance() << ", Travel time: " << exits[ii]->getLifeDistance() << " years.";
				}
				std::cout << "\n";
			}
		}
		cout << "\n"; 
	}
	if (items.size() > 0) {
		std::cout << "There are some things here:\n";
		for (int ii = 0; ii < items.size(); ii++) {
			std::cout << items[ii]->getName() << "\n";
		}
	}
	cout << "\n"; 

}
void Area::printLongDescription(){
		std::cout << longDescription << "\n";
		firstEntry = false;
		if(items.size() > 0){
			std::cout << "There are some items here:\n";
			for(int ii=0; ii < items.size(); ii++){
				std::cout << items[ii]->getName() << "\n";
			}
		}
		//Add logic to print description of items here
}
void Area::look(){
	if(dark){
		std::cout << "It is very dark in here you don't seem to be able to see much.\n";
		return;
	}
	printDescription();
}

Item* Area::getItem(std::string targetName){
	Item* itemPtr;
	for(int ii=0; ii < items.size(); ii++){
		if(items[ii]->getName().compare(targetName)==0){
			itemPtr = items[ii];
			return itemPtr;
		}
	}
	return NULL;
}

Item* Area::takeItem(std::string target){
	Item* itemPtr;
	for(int ii=0; ii < items.size(); ii++){
		if(items[ii]->getName().compare(target)==0 && items[ii]->isTakeable() ){
			itemPtr = items[ii];
			items.erase(items.begin()+ii);
			return itemPtr;
		}
	}
	return NULL;
}

void Area::dropItem(Item* thing){
	items.push_back(thing);
}


bool Area::hasExit(std::string target){
	int exitValid = false;
	for(int ii=0; ii < exits.size(); ii++){
		if(exits[ii]->getName().compare(target)==0){
			exitValid = true;
			break;
		}
	}
	return exitValid;
}


Exit* Area::getExit(std::string targetName){
	for(int ii=0; ii < exits.size(); ii++){
		if(exits[ii]->getName().compare(targetName)==0){
			return exits[ii];
		}
	}
	return NULL;
}

bool Area::lookAt(std::string targetName){
	for(int ii=0; ii < items.size(); ii++){
		if(items[ii]->getName().compare(targetName)==0){
			items[ii]->lookAt();
			return true;
		}
	}
	return false;
}

bool Area::hasItem(std::string targetName){
for(int ii=0; ii < items.size(); ii++){
		if(items[ii]->getName().compare(targetName)==0){
			return true;
		}
	}
	return false;

}

void Area::addExit(Exit* exit){
	exits.push_back(exit);
}
void Area::addItem(Item* item){
	items.push_back(item);
}


void Area::setLongDescription(std::string fileName){
	//from data.h
	longDescription = fileName;
}
void Area::setShortDescription(std::string fileName){
	//from data.h
	shortDescription= fileName;
}


std::vector<Item*> Area::getItems(){
        return items;
}
bool Area::getAreaEntry() {
        return firstEntry;
}

std::string Area::getName(){
	return areaName;
}

void Area::setEntry(int entry){
	bool fEntry;
	if(entry == 1) {
		fEntry = true;
	}
	else{
		fEntry = false;
	}
	firstEntry = fEntry;
}

bool Area::eraseItem(std::string targetName) {
	for (int ii = 0; ii < items.size(); ii++) {
		if (items[ii]->getName().compare(targetName) == 0) {
			items.erase(items.begin() + ii);
			return true;
		}
	}
	return false;
}
