#ifndef HUMAN_H 
#define HUMAN_H
#include "Item.h"
#include "Player.h"
#include <fstream>
class Human : public Item
{
private: 

/* Inherited attributes for reference
	std::string name; 
	std::string description; 
	bool breakable; 
	bool takeable; 
	bool edible; 
	bool wearable; 
*/
	bool hostile; 
	std::vector<std::string> speech; 
	
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
	Human(std::string nameIn, std::string descriptionIn, bool takeableIn);
	void talk();
	void setSayings(); 
	void aggravate(){hostile = true;}
	void makePeace(){hostile = false;}
	bool isHostile(){return hostile;}
	void attack(Player* player); 
	
}; 

#endif
