#ifndef AREA_H
#define AREA_H

#include <string>
#include <vector>
#include "Action.h"

using std::string;
using std::vector;

class Item;
class Exit;

class Area
{
private:
	string areaName;
	string longDescription;
	string shortDescription;
	string uIDisplay;
	vector<Exit*> exits;
	Exit* launchExit = NULL;
	vector<Item*> items;
	vector<Action> availableActions;
	string longFileNames;
	string shortFileNames;
	string uiFileName;
	bool firstEntry = true;
	bool dark;
	bool oxygen;
	bool space;
	string loadFile(string fileName);
public:
	//Adding constructor back
	Area(std::string name, std::string shortFileName, std::string longFileName, std::string interFile, bool isDark=false, bool hasOxygen=true, bool isSpace=false);
	void printDescription();
	void look();
	Item* takeItem(std::string targetName);
	Item* getItem(std::string targetName);
	void dropItem(Item* thing);
	bool hasExit(std::string target);
	Exit* getExit(std::string targetName);
	bool lookAt(std::string targetName);

	bool isDark(){return dark;}
	void setDark(bool tf){dark = tf;}
	bool hasOxygen(){return oxygen; }
	void setOxygen(bool tf){oxygen = tf; }
	void printLongDescription();
	void addExit(Exit* exit);
	void addItem(Item* item);
	void setLongDescription(std::string text);
	void setShortDescription(std::string text);
	bool hasItem(std::string targetName);
	void setLaunchExit(Exit* exit){launchExit= exit;}
	Exit* getLaunchExit(){return launchExit;}
	void setEntry(int entry);
	//Prints all of the items in the Area + the Area name -- done for save log.
    std::vector<Item*> getItems();
    std::string getLongFile() { return longFileNames; }
    std::string getShortFile() {return shortFileNames; }
    std::string getUIFile() {return uiFileName; }
    bool isfirstEntry() { return firstEntry; }
    bool getAreaEntry();
	std::string getName();
	bool isSpace() { return space; }
	bool eraseItem(std::string targetName);


};

#endif
