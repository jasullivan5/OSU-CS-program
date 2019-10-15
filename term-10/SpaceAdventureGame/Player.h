#ifndef PLAYER_H 
#define PLAYER_H
#include <string>
#include <vector> 
#include "Action.h"


using std::vector; 
//Forward declaration
class Area;
class Item;

class Player 
{
private: 
	vector<Item*> inventory;  
	vector<Item*> clothesWorn;  
	Area* location; 
	double lifeRemaining; 
	double gasRemaining; 

public: 
	bool drop(std::string targetName); 
	void addInventory(Item *item);
	bool take(std::string targetName); 
	Area* getLocation();
	std::string getLocationName();
	//Needed because of dynamic loadgame 
	void setVars(double gas, double life); 
	void listInventory(); 
	void setLocation(Area* anArea);
	bool lookAt(std::string targetName); 
	double getLife(){ return lifeRemaining;} 
	double getGas(){ return gasRemaining;}
	void removeLife(int years){lifeRemaining -= years;}
	void removeGas(int gas){gasRemaining -= gas;}
	bool launch(); 
	bool eat(std::string targetName); 
	int findItem(std::string targetName); 
	void getPlayer();
	std::vector<Item*> getPlayerInventory();
	//When user chooses to "load game" adds an item object
	//void initializeInventory(std::vector<Item*> toAdd);	
	bool use(std::string itemName); 
	bool isWearing(std::string name); 
	bool hasItem(std::string name); 
	bool eraseItem(std::string targetName);
	void kill();
	bool wear(Item* item); 
	bool takeOff(std::string targetName); 
	std::vector<Item*> getPlayerWearing() { return clothesWorn; }
}; 

#endif
