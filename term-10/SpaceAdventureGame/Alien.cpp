#include "Alien.h"
#include <time.h>

Alien::Alien(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}
void Alien::attack(Player* player){
	srand(time(NULL)); 
	int flip = rand()%2; 
	if(flip){
		player->kill(); 
	}
}

void Alien::setSayings(){
	std::vector<string> fileLine;
	string say;
   	std::fstream loadFile;
   	loadFile.open("loadfiles/says.txt");
   	while(!loadFile.eof()){
      getline(loadFile, say);
      if(say==""){
        continue;
      }
      else{
         fileLine.push_back(say);
      }
   }
   loadFile.close();	
   speech=fileLine;
}

void Alien::talk(){
	int s=rand() % 10+0;
	std::cout<< speech[s] << "\n"; 
}




