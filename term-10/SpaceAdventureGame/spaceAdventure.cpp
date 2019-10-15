//Space Adventure Game main code for CS497
//Written by Tim Ip ONID # 933912896
//

#include <iostream>
#include <vector>
#include <time.h>
#include <map>
#include <algorithm>
#include "spaceAdventure.h"
#include "parser.h"
#include "Player.h"
//#include "testData.h"

using std::map;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::fstream;
using std::cin;
using std::fstream;
using std::transform;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"

// Make sure that the word_files folder is in the same directory as the executable
const string VERB_FILE_LIST = "./word_files/verb_files.txt";
const string NOUN_FILE_LIST = "./word_files/noun_files.txt";

int main() {
	srand(time(NULL));

	enum Verb { look, move, help, inventory, lookAt, take, drop, fire, push, mine, launch, land, eat, bow, say, use, invalid, savegame, wear, status, quit };
	map<string, Verb> verbMap = {
		{"look",look},{"move",move},{"help",help},{"inventory",inventory},{"look at",lookAt},{"take",take},{"drop",drop},{"fire",fire},{"push",push},{"mine",mine},{"launch",launch},
		{"land",land},{"eat",eat},{"bow",bow},{"say",say},{"use",use},{"",invalid},{"savegame",savegame},{"wear",wear},{"status",status},{"quit",quit}
	};

	//Initialize parser
	Parser parser;
	int verb;
	string noun, command;
	if (parser.loadFiles(VERB_FILE_LIST, NOUN_FILE_LIST) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	// Welcome message
	vector<Item*> items = loadItems();
	Player player;
	vector<Area*> planets;
	vector<Exit*> exits;
	string userChooses;
	cout << "Welcome to the space adventure\n What do you want to do? \n Load Game\n New Game\n";
	getline(cin, userChooses);
	transform(userChooses.begin(), userChooses.end(), userChooses.begin(), tolower);
	//load the game
	string choice;
	int val=0;
	if(userChooses == "loadgame" || userChooses =="load" || userChooses == "load game") {
		cout << "Are you sure?" << endl;
		getline(cin, choice);
		if((choice == "yes") || (choice=="Yes") || (choice=="y") || (choice=="y")) {
			cout << "Loading game..." << endl;
			val=1; 
		}
	}
	if(val==1) {
		int fileReturn;
		fileReturn = open_log();
		if(fileReturn == 1 ) {
			vector<string> savedLines=openLoadFile("loadfiles/areas.txt");
			planets=loadPlanets(savedLines, items, 1);
			player=createNewPlayer(savedLines, planets, items);
			player.getLocation()->addItem(getItemPtr("Spaceship", items));
			if(player.getLocation()->getName() == "Mercury") {
				((Spaceship*)(getItemPtr("Spaceship", items)))->setFixes(0);
			}
		}
		else {
			vector<string> savedLines=openLoadFile("saveLog.txt");
			planets=loadPlanets(savedLines, items, 3);
			player=loadOldPlayer(savedLines, planets, items);
		}
	}
	else{
		vector<string> savedLines=openLoadFile("loadfiles/areas.txt");
		planets=loadPlanets(savedLines, items, 1);
		player=createNewPlayer(savedLines, planets, items);
		player.getLocation()->addItem(getItemPtr("Spaceship", items));
		if(player.getLocation()->getName() == "Mercury") {
			((Spaceship*)(getItemPtr("Spaceship", items)))->setFixes(0);
		}
	}
	//Create exit objects + setting exits for each planet
	exits=createExits(planets);
	setPlanetExits(planets, exits);
	Alien* pAlien=dynamic_cast<Alien*>(getItemPtr("Alien", items));
	pAlien->setSayings();
	Human* pHuman=dynamic_cast<Human*>(getItemPtr("Human", items));
	pHuman->setSayings();
	//Welcome text displaying object of the game
	getWelcome(&player);

	bool gameOver = false;
	int turnCounter = 0;
	int suffocationCounter = 4;
	int attackCounter = 3; 
	while (!gameOver && player.getLife() > 0 && player.getGas() >= 0 ){
		// Cold area check
		if (!player.isWearing("Jacket") && (player.getLocation()->getName().compare("Pluto") == 0 ||
			player.getLocation()->getName().compare("Uranus") == 0 ||
			player.getLocation()->getName().compare("Neptune") == 0)) {
			if (turnCounter > 4) {
				cout << "You didn't put your jacket on. You froze to DEATH.\n";
				gameOver = true;
				break;
			}
			cout << "It is extremely cold here. Better put on a jacket.\n";
			turnCounter++;
		}

		// No oxygen check
		if( (player.getLocation()->getName().compare("Lost Moon") == 0 || player.getLocation()->getName().compare("Jupiter") == 0) && !player.getLocation()->hasOxygen()){
			if(suffocationCounter < 0){
				gameOver = true;
				cout << "Oh no looks like you suffocated.\n";
				break;
			}
			cout << "You don't seem to be able to breath here.\n";
			suffocationCounter--;
		}

		// Process user input
		verb = invalid;
		noun.clear();
		command.clear();
		cout << "Please enter a command: ";
		getline(cin, command);
		parser.processInput(command);
		verb = verbMap[parser.getVerb()];
		if (parser.getNouns().size() > 0) {
			noun = parser.getNouns()[0];
		}
	
		Area* location = player.getLocation();
		switch (verb){
			//look around current location
			case look:
				player.getLocation()->look();
				break;

			//go somewhere, check exit is accessible and go there
			case move:
				if(!location->isSpace() && noun.compare("Space")!=0) {
					if(player.isWearing("Shoes")) {
						moveFxn(noun, player, getItemPtr("Spaceship", items), false);
						attackCounter = 2; 
					}
					else {
						cout << "It's hard to walk anywhere on this surface in your bare feet...\n";
					}
				}
				else{
					cout << "You must use the Launch or Land commands for travel to and from space.\n";
				}
				break;

			case help:
				cout << "Try one of the following commands: \n Look \n Look at \n Move \n Inventory \n Take \n Drop \n Fire \n Eat \n Bow \n Say \n Use \n Push \n Launch \n Land \n Wear \n Mine \n Status \n Quit \n Savegame \n";
				break;

			case inventory:
				player.listInventory();
				break;

			// If an item action is requested check if the item is available
			//Check every inventory item for the item name
			case lookAt:
				if(player.lookAt(noun)){
					break;
				}else if(player.getLocation()->lookAt(noun)){
					// If player looks at rover for the first time, add transmitter to area's items
					if (noun.compare("OpportunityRover") == 0 && !itemExists(planets, player, "Transmitter")) {
						player.getLocation()->addItem(getItemPtr("Transmitter", items));
						cout << "A transmitter is attached to Opportunity Rover.\n";
					}
					break;
				}else{
					cout << "Sorry you can't look at that right now\n" ;
					break;
				}

			case take:
				if(player.take(noun)){
					break;
				}else{
					cout << "Sorry you don't seem to be able to take that right now.\n";
					break;
				}

			case drop:
				if(player.drop(noun)){
					cout << "You dropped the " << noun << ".\n";
					break;
				} else {
					cout << "Sorry you don't seem to have that in your inventory... soooo you can't drop it.\n";
					break;
				}

			case fire:
				//Throw something
				if(noun.compare("Shoes")!=0){
					if(player.drop(noun)){
						cout << "You threw the " << noun << ".\n";
						break;
					} else {
						cout << "Sorry you don't seem to have that in your inventory... soooo you can't throw it.\n";
					}
				//Throw your shoe...
				}else if(player.drop("Shoes")){
					 cout<<"You threw your shoe.\n"; 
					if(location->hasItem("Alien")){
						Alien* pAlien = dynamic_cast<Alien*>(location->getItem("Alien")); 
						if(pAlien != nullptr){
							//((Alien *)(location->getItem("Alien")))->aggravate(); 
							pAlien->aggravate(); 
							cout << "Looks like that aggravated the alien, it is now hostile towards you.\n"; 
						}
					}
				}
				break;

			case push:
				if(noun.compare("Alien")==0 && location->hasItem("Alien")){
					Alien* pAlien = dynamic_cast<Alien*>(location->getItem("Alien")); 
					if(pAlien != nullptr){
						pAlien->aggravate(); 
						cout << "Looks like that aggravated the alien, it is now hostile towards you.\n"; 
					}
				}else{
					cout << "You push it but it didn't seem to care.\n"; 
				}
				break;

			case mine:
				if(!player.use("PickAxe")){
					cout << "Looks like you can't mine right now.\n"; 
				}
				break;

			case launch:
				// IF spaceship is present launch to space
				if(location->hasItem("Spaceship") && location->getLaunchExit() != NULL){
					if(player.isWearing("Shoes") && !location->isSpace()){
						moveFxn(noun, player, getItemPtr("Spaceship", items), true);
						turnCounter = 0;
						suffocationCounter = 4;
					}else{
						cout << "You're having trouble walking to your space ship in your bare feet...\n"; 
					}
				}else{
					cout << "Sorry you don't appear to be able to launch from here.\n";
				}
				break;

			case land:
				//If in space land on specified planet
				if(location->isSpace()){
					if(noun.compare("Earth")==0){
						if(((Spaceship*)getItemPtr("Spaceship", items))->isFixed()){
							moveFxn(noun, player, getItemPtr("Spaceship", items), true);
							attackCounter = 2; 
						} else {
							cout << "Sorry your space ship isn't fixed yet. It won't survive entry into Earth's atmosphere.\n"; 
						}
					}else{
						moveFxn(noun, player, getItemPtr("Spaceship", items), true);
					}
				}else{
					cout << "You will need to be flying in space before you can land your ship.\n";
				}
				break;

			case eat:
				if (noun.compare("Mushroom") != 0 || !player.use("Mushroom")) {
					cout << "You can't eat that.\n";
				}
				break;

			case bow:
				if(location->hasItem("Alien")&& player.hasItem("Crysallith")){
					Alien* pAlien = dynamic_cast<Alien*>(location->getItem("Alien")); 
					if(pAlien != nullptr ){
						pAlien->makePeace(); 
					}
					read_uif_files("UIf_files/crystal.txt");
					cout <<"You bow to the Alien with Crysallith in your hands. It seems to accept your peace offering.\n"; 
				}else if(location->hasItem("Alien")){
					Alien* pAlien = dynamic_cast<Alien*>(location->getItem("Alien")); 
					if(pAlien != nullptr ){
						pAlien->makePeace(); 
					}
					cout <<"You bow to the Alien, he seems to accept your deference despite your lack of an offering.\n";  
				}else{
					cout << "You bowed, but no one seemed to notice.\n"; 	
				}
				break;

			case say:
				if(noun=="Alien" && location->hasItem("Alien") ){
					read_uif_files(UI_ALIEN);
					//Alien* pAlien = dynamic_cast<Alien*>(location->getItem("Alien")); 
					if(pAlien != nullptr ){
						cout << "The alien replies: "; 
						pAlien->talk(); 
					}
				} else if (noun == "Human"){
					//Human* pHuman= dynamic_cast<Human*>(location->getItem("Human")); 
					if(pHuman!= nullptr ){
						cout << "The human replies: "; 
						pHuman->talk(); 
					}
				} else {
					cout << "You speak but no one seems to be around to hear you.\n"; 
				}
				break;

			case use:
				player.use(noun); 
				break; 

			case invalid:
				std::cout << "Uh that doesn't make sense try something else.\n";
				break;

			case savegame:

				saveGame(&player, planets, ((Spaceship *)getItemPtr("Spaceship", items)));
				break;

			case wear:
				if ((noun.compare("Shoes") == 0 || noun.compare("Jacket") == 0) &&
					player.hasItem(noun))
				{
					player.use(noun);
				}
				else {
					cout << "You can't wear that.\n";
				}
				break;
			case status:
				cout << "You have " << player.getLife() << " years to live and " << player.getGas() << " gas left.\n";
				break;
			case quit:
				cout << "Are you sure you want to quit and exit the game? Type yes or no: ";
				getline(cin, command);
				transform(command.begin(), command.end(), command.begin(), tolower);
				if(command.compare("yes") == 0) {
					cleanObjects(items, planets, exits);
					return 0;
				}
				break;
		}
		if(player.getLocation()->hasItem("Alien")){
			attackCounter--;
			//Alien* pAlien = dynamic_cast<Alien*>(location->getItem("Alien")); 
			if(pAlien != nullptr && pAlien->isHostile() && attackCounter < 0){
				pAlien->attack(&player); 
				if(player.getLife() <= 0){
					cout << "The alien pulls out a gun looking device and fires a beam at you.... Anddddd you are now a pile of ashes.\n"; 
				}else{
					cout << "The alien shot a laser beam at you and barely missed. Looks like you've worn out your welcome.\n";
				}
			}
		}
		cout << "\n";

	}
	read_uif_files(UI_FAILURE);
	cleanObjects(items, planets, exits);
	return 0;
}


void moveFxn(string noun, Player &player, Item *spaceship, bool spaceMove){
	Area* location = player.getLocation();
	bool exitValid = location->hasExit(noun);
	if(exitValid){
		Exit* targetExit = location->getExit(noun);
		if (targetExit->getName().compare("Space") == 0) {
			player.getLocation()->eraseItem("Spaceship");
		}
		if(spaceMove){
			player.removeLife(targetExit->getLifeDistance());
			player.removeGas(targetExit->getGasDistance());
		}
		if(player.getLife() < 1){
			cout << "Oh no! It appears you died of old age before reaching Earth.\n";
		}else if (player.getGas() < 0){
			cout << "Oh no! It appears you ran out of gas and are stranded in space.\n";
		}else{
			player.setLocation(location->getExit(noun)->getArea());
			if (location->getName().compare("Space") == 0) {
				player.getLocation()->addItem(spaceship);
			}
			player.getLocation()->look();
			read_uif_files(player.getLocation()->getUIFile());
		}
	} else {
		std::cout << "Sorry that doesn't appear to be a place you can get to from here. \n";
	}
}

void read_uif_files(string fileName){
	int num= rand()% 4 + 0;

	fstream r_file;
	string file_read;
	const string readFile=fileName;
	r_file.open(readFile);
	if(r_file.is_open()){ 
		while(!r_file.eof()) {
			getline(r_file, file_read);
			if(num == 0) { cout << RED << file_read << RESET << endl; }
			if(num ==1) { cout << BLACK << file_read << RESET << endl; }
			if(num ==2) { cout << GREEN << file_read << RESET << endl; }
			if(num ==3) { cout << MAGENTA << file_read << RESET << endl; }
			if(num ==4) { cout << BLUE << file_read << RESET << endl; }
		}
		r_file.close();
	}
	else{
		cout << "File load error" << endl;
	}	
}
std::string get_file_data(std::string fileName){
	fstream r_file;
	string file_read;
	const string readFile= "descriptorFiles/"+fileName;
	r_file.open(readFile);
	while(!r_file.eof()) {
		getline(r_file, file_read);
		//cout << file_read << endl;
	}
	cout << file_read << endl;
	r_file.close();
	return file_read;
}
void getWelcome(Player* player){

	if(player->getLocation()->getName() == "Uranus"){
		get_file_data("welcome_1.txt");
		cout << "As of now you have: " << player->getLife() << " years left" << endl;
	}
	else {
		get_file_data("welcome_0.txt");
		cout << "As of now you have: " << player->getLife() << " years left and " << player->getGas() << " million miles left in the tank\n\n" << endl;
	}
	read_uif_files(player->getLocation()->getUIFile());
	player->getLocation()->printDescription();
}

bool itemExists(vector<Area*> planets, Player player, string item) {
	if (player.hasItem(item)) {
		return true;
	}
	for (int i = 0; i < planets.size();i++) {
		if (planets[i]->hasItem(item)) {
			return true;
		}
	}
	return false;
}

Item* getItemPtr(string itemName, vector<Item*> itemList) {
	for (int i = 0; i < itemList.size(); i++) {
		if ((itemList[i]->getName()).compare(itemName) == 0) {
			return itemList[i];
		}
	}
	return NULL;
}
