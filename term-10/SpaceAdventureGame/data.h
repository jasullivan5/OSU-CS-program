#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Pickaxe.h"
#include "Flashlight.h"
#include "Mushroom.h"
#include "PowerCrystal.h"
#include "Spaceship.h"
#include "Doohickey.h"
#include "Transmitter.h"
#include "Alien.h"
#include "Human.h"
#include "ScrewDriver.h"
#include "Exit.h"
//#include "Player.h"
//#include "Area.h"
class Player;
class Area; 
class Item;



//UI functions --to be pretty
void display_planet();
void display_descriptions();
//i.e. game over, start menue
void display_gameInstances();
//void createNewPlanets(std::vector<Area*> planets, std::vector<Item> items);
Player createNewPlayer(std::vector<string> savedLines, std::vector<Area*> planets, std::vector<Item*> items);
//save log file conditions, will be called in the "main" game
void saveGame(Player* player1, std::vector<Area*> planets, Spaceship* space); 
//Parse the save_log.txt and place in constructor for the areas + user
int open_log();
Player loadOldPlayer(std::vector<std::string> savedLines, std::vector<Area*> planets, std::vector<Item*> items);
//void loadOldPlanets(std::vector<std::string> savedLines, std::vector<Area> planets, std::vector<Item> items);
std::vector<std::string> openLoadFile(std::string fileName);
std::vector<Area*> loadPlanets(std::vector<string> savedLines, std::vector<Item*> items, int start);
std::vector<Exit*> createExits(std::vector<Area*> areas);
std::vector<Item*> loadItems();
void setPlanetExits(std::vector<Area*> planets, std::vector<Exit*> exits);
void cleanObjects(std::vector<Item*> items, std::vector<Area*> areas, std::vector<Exit*> exits);
#endif // DATA_H
