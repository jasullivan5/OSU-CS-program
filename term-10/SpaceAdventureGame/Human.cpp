#include "Human.h"
#include <time.h>

Human::Human(std::string nameIn, std::string descriptionIn, bool takeableIn)
	: Item(nameIn, descriptionIn, takeableIn) {}

void Human::attack(Player* player){
	srand(time(NULL)); 
	int flip = rand()%2; 
	if(flip){
		player->kill(); 
	}
}
void Human::setSayings(){
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

void Human::talk(){
	int s=rand() % 10+0;
	std::cout<< speech[s] << "\n"; 
}



