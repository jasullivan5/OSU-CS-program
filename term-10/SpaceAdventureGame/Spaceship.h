#ifndef SPACESHIP_H 
#define SPACESHIP_H
#include "Item.h"

class Spaceship : public Item
{
private: 

	int fixesNeeded = 3; 
/* Inherited attributes for reference
	std::string name; 
	std::string description; 
	bool breakable; 
	bool takeable; 
	bool edible; 
	bool wearable; 
*/
	
public: 
/* Inherited methods for reference
	void lookAt(); 
	std::string getName(); 
	bool isTakeable(); 
	bool isEdible(){ return edible; }
	void setEdible(bool edible); 
	bool isWearable(){return wearable;}
	void setWearable(bool wear){wearable = wear;}
	Item(std::string name, std::string description, bool takeable); 
*/
	Spaceship(std::string nameIn, std::string descriptionIn, bool takeableIn);
	void use(Area* location, Player* player);
	void fix() { cout << "You have made repairs to your ship.\n"; fixesNeeded--; }
	void setFixes(int fixesIn){ fixesNeeded = fixesIn; }
	bool isFixed();
	int getFixes() { return fixesNeeded; }
	
}; 

#endif
