/******************************************************************************
 * FILE NAME:	sullijos.adventure.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 2 - adventure
 * START DATE:	2/12/2018
 * DESCRIPTION: gets room data from files in directory, 
 * 		sullijos.rooms.<PROCESS ID>. Room data is used
 * 		for a text adventure game. The game displays the current room,
 * 		rooms connected to the current room that the player can travel
 * 		to, and a prompt in which the player can enter the room they
 * 		wish to travel to. The program validates user input. If input
 * 		is a valid room name, the user will travel to the room,
 * 		otherwise the program displays an error and asks for a connected
 * 		room name again. Once the player reaches the end room, the game
 * 		congratulates the user, displays the number of steps and the path
 * 		the the user took and exits.
 *****************************************************************************/
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Constants and Macros
#define DIRECTORY_NAME_PREFIX "sullijos.rooms."
#define STRING_BUFFER_SIZE 256
#define MAX_LINE_LENGTH 23
#define NAME_LENGTH 9 
#define TYPE_LENGTH 12
#define NUMBER_OF_ROOMS 7
#define MAX_CONNECTIONS (NUMBER_OF_ROOMS - 1)
#define TITLE_SIZE 11
#define INITIAL_CAPACITY 2

// Structs

struct Room {
char name[NAME_LENGTH];
int numberOfConnections;
struct Room* connections[MAX_CONNECTIONS];
char type[TYPE_LENGTH];
};

struct GameState {
	char** visitedRooms;
	int visitedRoomsSize;
	int visitedRoomsCapacity;
	struct Room* currentRoom;
};

struct InputBuffer {
	char* buffer;
	size_t size;
};

// Function Prototypes
void* mainThreadFunction(void* argument);
void* makeTimeFile(void* argument);
void getTime(char* timeString);
void writeTimeToFile(char* filePath, char* time);
void getRoomData(struct Room* roomArray);
void getNewestRoomDirectoryName(char* stringBuffer);
int isRoomDirectory(struct dirent* entry);
struct dirent* getNewerEntry(struct dirent* entry1, struct dirent* entry2);
int getEntryTimeStamp(struct dirent* entry);
void getFileNames(char* directoryName, char** bufferArray);
int isCurrentOrParentDirectory(struct dirent* directoryEntry);
void makeRoomStructs(struct Room* roomArray, char** fileNameArray, char* directoryName);
void initializeRooms(struct Room* roomArray);
void setNameAndType(struct Room* room, char* filePath);
void isNameLine (struct Room* room, char* fileLine);
void isTypeLine (struct Room* room, char* fileLine);
void setConnections(struct Room* room, char* filePath, struct Room* roomArray);
void isConnectionLine(struct Room* room, char* fileLine, struct Room* roomArray, int* index);
void runGame(struct Room* roomArray);
void initializeInputBuffer(struct InputBuffer* inputBuffer);
void initializeGameState(struct GameState* gameState, struct Room* roomArray);
void addVisitedRoom(struct GameState* gameState);
void setCapacity(struct GameState* gameState);
struct Room* getStartRoom(struct Room* roomArray);
int isEndRoom(struct Room* room);
void showRoomData(struct Room* room);
void printConnections(struct Room* room);
void getUserInput(struct InputBuffer* inputBuffer);
void printTime();
int isValidInput(struct InputBuffer* input, struct Room* room, int* connectionIndex);
void endGame(struct GameState* gameState);

// Declare mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



// main function starts threading process
int main() {
	// Create main game thread
	pthread_t mainThread;
	pthread_create(&mainThread, NULL, mainThreadFunction, NULL);

	// Join main thread at end to clean up before return
	pthread_join(mainThread, NULL);
	return 0;
}



/********** FUNCTIONS FOR TIME THREAD **********/

void* makeTimeFile(void* argument) {
	pthread_mutex_lock(&mutex); // lock mutex

	// Get Time
	char timeString[STRING_BUFFER_SIZE];
	memset(timeString, '\0', STRING_BUFFER_SIZE);
	getTime(timeString);
	
	// Write Time to File
	writeTimeToFile("currentTime.txt", timeString);
	pthread_mutex_unlock(&mutex); // unlock mutex
	return NULL;
}

void getTime(char* timeString) {
	// Get raw time data
	time_t timeVariable;
	time(&timeVariable);

	// send raw time data to struct that provides formatting
	struct tm* timeStruct;
	timeStruct = localtime(&timeVariable);

	// Format time and copy to string buffer
	strftime(timeString, STRING_BUFFER_SIZE, "%I:%M%P, %A, %B %d, %Y", timeStruct);
}

void writeTimeToFile(char* filePath, char* time) {
	// Open file
	FILE* file = fopen(filePath, "w");

	// write time to file, ignoring a zero at beginning if one is present
	if (time[0] == '0') {
		fprintf(file, "%s\n", &time[1]);
	}
	else {
		fprintf(file, "%s\n", time);
	}

	// close file
	fclose(file);
}



/********** FUNCTIONS FOR RUNNING GAME **********/

// mainThread handles mutex and calls functions for game
void* mainThreadFunction(void* argument) {
	pthread_mutex_lock(&mutex); // lock mutex

	// declare array of structs to hold room data
	struct Room roomArray[NUMBER_OF_ROOMS];

	// Get room data from files
	getRoomData(roomArray);

	// Run game
	runGame(roomArray);

	pthread_mutex_unlock(&mutex); // unlock mutex
	return NULL;
}



/* FUNCTIONS FOR GETTING ROOM DATA */

// Get directory name, get files, and make room structs 
void getRoomData(struct Room* roomArray) {
	// Get directory name
	char directoryName[STRING_BUFFER_SIZE];
	memset(directoryName, '\0', STRING_BUFFER_SIZE * sizeof(char));
	getNewestRoomDirectoryName(directoryName);

	char* fileNames[NUMBER_OF_ROOMS]; // char buffer for file name

	// allocate memory for names
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
		fileNames[i] = malloc(NAME_LENGTH * sizeof(char));
	}

	// get names
	getFileNames(directoryName, fileNames);
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
	}

	// Make Rooms
	makeRoomStructs(roomArray, fileNames, directoryName);	

	// Free room name memory
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
		free(fileNames[i]);
	}
}

void getNewestRoomDirectoryName(char* stringBuffer) {
	// Open current directory
	DIR* directory = opendir(".");
	
	// Declare directory entry pointers
	struct dirent* currentEntry = NULL;
	struct dirent* newestEntry = NULL;

	// read entries from current directory
	while ((currentEntry = readdir(directory))) {

		// if the entry is a room directory...
		if (isRoomDirectory(currentEntry)) {
			// compare it to the current newest directory and choose newer of the two
			newestEntry = getNewerEntry(newestEntry, currentEntry);
		}
	}

	// put name of newest room directory in string buffer
	strcpy(stringBuffer, newestEntry->d_name);

	// close directory
	closedir(directory);
}


// Check if directory is a room directory by checking name prefix
int isRoomDirectory(struct dirent* entry) {
	int prefixLength = strlen(DIRECTORY_NAME_PREFIX);
	if (strncmp(entry->d_name, DIRECTORY_NAME_PREFIX, prefixLength) == 0) {
		return 1;
	}
	return 0;
}								

// Compare the time stamp of two directory entries and return pointer to newest one
struct dirent* getNewerEntry(struct dirent* entry1, struct dirent* entry2) {
	// Check if either entry is NULL first
	if (entry1 == NULL) {
		return entry2;
	}
	else if (entry2 == NULL) {
		return entry1;
	}
	else {
		// Compare time stamps
		int entry1TimeStamp = getEntryTimeStamp(entry1);
		int entry2TimeStamp = getEntryTimeStamp(entry2);
		if (entry1TimeStamp >= entry2TimeStamp) {
			return entry1;
		}
		else {
			return entry2;
		}
	}
}

// Get time that directory entry was created
int getEntryTimeStamp(struct dirent* entry) {
	struct stat buffer;
	stat(entry->d_name, &buffer);
	return ((int) buffer.st_mtime);
}

// Get file names from rooms directory
void getFileNames(char* directoryName, char** bufferArray) {
	DIR* directory = opendir(directoryName); // open room directory

	// iterate through room directory entries
	struct dirent* entry = NULL;
	int index = 0;
	while ((entry = readdir(directory))) {

		// copy file names into string array, excluding current and parent directories
		if (!isCurrentOrParentDirectory(entry)) {
			strcpy(bufferArray[index], entry->d_name);
			index++;
		}
	}
	closedir(directory); // close directory
}

// Checks to see if directory entry is current or parent directory
int isCurrentOrParentDirectory(struct dirent* directoryEntry) {
	if (directoryEntry->d_name[0] == '.') {
		return 1;
	}
	else {
		return 0;
	}
}


void makeRoomStructs(struct Room* roomArray, char** fileNameArray, char* directoryName) {
	// Initialize rooms
	initializeRooms(roomArray);

	// Make file path
	int filePathLength = strlen(directoryName) + NAME_LENGTH + 1;
	char filePath[filePathLength];

	// iterate through rooms, setting names and types
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {	
		memset(filePath, '\0', filePathLength);
		sprintf(filePath, "%s/%s", directoryName, fileNameArray[i]);
		setNameAndType(&roomArray[i], filePath);
	}

	// iterate through rooms, setting connections
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
		memset(filePath, '\0', filePathLength);
		sprintf(filePath, "%s/%s", directoryName, fileNameArray[i]);
		setConnections(&roomArray[i], filePath, roomArray);
	}
}

// Initialize room struct members
void initializeRooms(struct Room* roomArray) {	
	// iterate through rooms
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {	

		// clear name memory	
		memset(roomArray[i].name, '\0', NAME_LENGTH);

		// set connectiosn to 0
		roomArray[i].numberOfConnections = 0;

		// set connection pointers to NULL
		int j;
		for (j = 0; j < MAX_CONNECTIONS; ++j) {
			roomArray[i].connections[j] = NULL;
		}
		
		// clear type memory
		memset(roomArray[i].type, '\0', TYPE_LENGTH);
	}
}


// Get name and type from file and set to struct members
void setNameAndType(struct Room* room, char* filePath) {
	FILE* file = fopen(filePath, "r"); // open file
	
	// declare buffer and clear memory
	char lineBuffer[MAX_LINE_LENGTH];
	memset(lineBuffer, '\0', MAX_LINE_LENGTH);

	// read lines from file
	while (fgets(lineBuffer, MAX_LINE_LENGTH, file)) {

		// remove newline from line read from file
		lineBuffer[strcspn(lineBuffer, "\n")] = '\0';

		// If line is a name or type line, set name or type
		isNameLine(room, lineBuffer);
		isTypeLine(room, lineBuffer);

		// clear memory of buffer for reuse
		memset(lineBuffer, '\0', MAX_LINE_LENGTH);
	}
	fclose(file); // close file
}

// If line is name line, set it to name member of room passed in
void isNameLine (struct Room* room, char* fileLine) {
	if (strncmp(fileLine, "ROOM NAME: ", TITLE_SIZE) == 0) {
		strcpy(room->name, &fileLine[TITLE_SIZE]);
	}
}

// If line is type line, set to type member of room passed in
void isTypeLine (struct Room* room, char* fileLine) {
	if (strncmp(fileLine, "ROOM TYPE: ", TITLE_SIZE) == 0) {
		strcpy(room->type, &fileLine[TITLE_SIZE]);
	}
}

// Set a rooms array of connection pointers to connections read from file
void setConnections(struct Room* room, char* filePath, struct Room* roomArray) {
	FILE* file = fopen(filePath, "r"); // open file

	// declare and clear buffer 
	char lineBuffer[MAX_LINE_LENGTH];
	memset(lineBuffer, '\0', MAX_LINE_LENGTH);
	int connectionIndex = 0; // for tracking index of connections

	// read lines from file
	while (fgets(lineBuffer, MAX_LINE_LENGTH, file)) {

		// remove new line from line read in from file
		lineBuffer[strcspn(lineBuffer, "\n")] = '\0';

		// If line is a connection line, set connection in rooms connection array
		isConnectionLine(room, lineBuffer, roomArray, &connectionIndex);

		memset(lineBuffer, '\0', MAX_LINE_LENGTH); // clear buffer memory for reuse
	}

	fclose(file); // close file
}

// if line is a connection line, st connection in rooms connection array
void isConnectionLine (struct Room* room, char* fileLine, struct Room* roomArray, int* index) {
	// check if line is a connection line
	if (strncmp(fileLine, "CONNECTION ", TITLE_SIZE) == 0) {

		// copy name of connected room into buffer
		char connectionName[NAME_LENGTH];
		memset (connectionName, '\0', NAME_LENGTH);
		strcpy(connectionName, &fileLine[TITLE_SIZE + 3]); // +3 to exclude ":# " characters

		// search for room struct in array and connect it to current room
		int i;
		for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
			if (strcmp(connectionName, roomArray[i].name) == 0) {
				room->connections[*index] = &roomArray[i];
				room->numberOfConnections++; 
			}
		}
		(*index)++; // increment connection index from calling function
	}
}



/********** FUNCTIONS FOR MANAGING GAME **********/

// Calls various functions for stepping through the game
void runGame(struct Room* roomArray) {

	// Game state struct tracks current room, number of steps, and path
	struct GameState gameState;
	initializeGameState(&gameState, roomArray);

	// Input buffer holds users input
	struct InputBuffer userInput;
	initializeInputBuffer(&userInput);

	// Continue game until end room is found
	while (! isEndRoom(gameState.currentRoom)) {

		// if last input was not "time" show current room data
		if (strcmp(userInput.buffer, "time") != 0) {
			showRoomData(gameState.currentRoom);
		}

		// Get input from user
		getUserInput(&userInput);

		int connectionIndex; // holds connection index of chosen room to travel to

		// Print the time if user's input is "time"
		if (strcmp(userInput.buffer, "time") == 0) {
			printTime();
		}

		// If input nonsense or not a connection of current room, print error message
		else if (! isValidInput(&userInput, gameState.currentRoom, &connectionIndex)) {
			printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}

		// If input is connection to current room, travel to that room and add it to rooms visited
		else {
			gameState.currentRoom = gameState.currentRoom->connections[connectionIndex];
			addVisitedRoom(&gameState);
		}
	}

	// End game when end room is found and free memory
	endGame(&gameState);
	free(gameState.visitedRooms);
	free(userInput.buffer);
}

// allocate memory for input buffer and clear it.
void initializeInputBuffer(struct InputBuffer* inputBuffer) {
	inputBuffer->size = STRING_BUFFER_SIZE;
	inputBuffer->buffer = malloc(inputBuffer->size * sizeof(char));
	memset(inputBuffer->buffer, '\0', inputBuffer->size);
}

// allocate memory for visited room and set current room to start room
void initializeGameState(struct GameState* gameState, struct Room* roomArray) {
	gameState->visitedRooms = malloc(INITIAL_CAPACITY * sizeof(char*));
	gameState->visitedRoomsSize = 0;
	gameState->visitedRoomsCapacity = INITIAL_CAPACITY;
	gameState->currentRoom = getStartRoom(roomArray);
}

// add visited room to visited room struct member array	
void addVisitedRoom(struct GameState* gameState) {

	// resize array for visited rooms if neccesary 
	if (gameState->visitedRoomsSize >= gameState->visitedRoomsCapacity){
		gameState->visitedRoomsCapacity = gameState->visitedRoomsCapacity * 2;
		setCapacity(gameState);
	}

	// add current room just traveled to to visited rooms
	gameState->visitedRooms[gameState->visitedRoomsSize] = gameState->currentRoom->name;
	gameState->visitedRoomsSize++; // increase step counter
}

// Set new capacity for visited rooms struct member array
void setCapacity(struct GameState* gameState) {
	// make new array of new size
	char** newVisitedRooms = malloc(gameState->visitedRoomsCapacity * sizeof(char*));
	
	// copy rooms visited from old array
	int i;
	for (i = 0; i < gameState->visitedRoomsSize; ++i) {
		newVisitedRooms[i] = gameState->visitedRooms[i];
	}

	// free memory for old array
	free(gameState->visitedRooms);

	// set point visitedRooms member to new visited rooms array
	gameState->visitedRooms = newVisitedRooms;
}

// iterate through rooms until start room is found and return pointer to it
struct Room* getStartRoom(struct Room* roomArray) {
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
		if (strcmp(roomArray[i].type, "START_ROOM") == 0) {
			return &roomArray[i];
		}
	}
	return NULL;
}

// check if room is end room
int isEndRoom(struct Room* room) {
	if (strcmp(room->type, "END_ROOM") == 0) {
		return 1;
	}
	return 0;
}

// display current location and possible connections on console
void showRoomData(struct Room* room) {
	printf("CURRENT LOCATION: %s\n", room->name);
	printf("POSSIBLE CONNECTIONS: ");
	printConnections(room);
}

// Print connections of room with proper formatting
void printConnections(struct Room* room) {
	int i;
	for (i = 0; i < room->numberOfConnections; ++i) {
		printf("%s", room->connections[i]->name);
		if((i + 1) < room->numberOfConnections) {
			printf(", ");
		}
		else {
			printf(".\n");
		}
	}
}

// Provide user with a prompt and copy there input to a buffer
void getUserInput(struct InputBuffer* inputBuffer) {
	memset(inputBuffer->buffer, '\0', inputBuffer->size);
	printf ("WHERE TO? >");
	getline(&inputBuffer->buffer, &inputBuffer->size, stdin);
	inputBuffer->buffer[strcspn(inputBuffer->buffer, "\n")] = '\0';
	printf ("\n");
}

// Create time thread, open current time file, and display contents on console
void printTime() {
	// create thread
	pthread_t timeThread;
	pthread_create(&timeThread, NULL, makeTimeFile, NULL);

	// unlock to allow time thread access to mutex, then lock agian when time thread is finished
	pthread_mutex_unlock(&mutex);
	pthread_join(timeThread, NULL);
	pthread_mutex_lock(&mutex);

	// Open file and print contents to console
	FILE* file = fopen("currentTime.txt", "r");
	char timeBuffer[STRING_BUFFER_SIZE];
	printf("%s\n", fgets(timeBuffer, STRING_BUFFER_SIZE, file));
	fclose(file); // close file
}

// check if input matches any possible connections 
int isValidInput(struct InputBuffer* input, struct Room* room, int* connectionIndex) {
	int i;
	for (i = 0; i < room->numberOfConnections; ++i) {
		if (strcmp(input->buffer, room->connections[i]->name) == 0) {
			*connectionIndex = i;
			return 1;
		}
	}
	return 0;
}

// Congratulate the player and show them their number of steps and path to victory		
void endGame(struct GameState* gameState) {
	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", gameState->visitedRoomsSize);
	int i;
	for (i = 0; i < gameState->visitedRoomsSize; i++) {
		printf("%s\n", gameState->visitedRooms[i]);
	}
}
