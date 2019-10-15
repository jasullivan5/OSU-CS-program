#ifndef DOOHICKEY_H 
#define DOOHICKEY_H
#include "Item.h"

class Doohickey : public Item
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
	Doohickey(std::string nameIn, std::string descriptionIn, bool takeableIn);
	void use(Area* location, Player* player);
	
}; 

#endif
