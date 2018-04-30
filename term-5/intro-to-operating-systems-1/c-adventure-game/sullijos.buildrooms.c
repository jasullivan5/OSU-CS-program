/******************************************************************************
 * FILE NAME:	sullijos.buildrooms.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 2 - adventure
 * START DATE:	2/5/2018
 * DESCRIPTION:	Creates a directory called sullijos.rooms.<PROCESS ID> and
 * 		creates seven room files within it. Each room file contains
 * 		data representing a room in an adventure game. Rooms are
 * 		assigned random names from a hard-coded list of ten names. The
 * 		rooms are assigned between three and six random connections to
 * 		other rooms. One room is randomly assigned as the start and
 * 		another as the end. A list containing a rooms name,
 * 		connections, and whether it is a start, middle, or end room is
 * 		written to each room file. These files are intended for use
 * 		with the sullijos.adventure.c program.
 *****************************************************************************/
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>

// Constants and Macros
#define STRING_BUFFER_SIZE 256
#define DIRECTORY_NAME_PREFIX "sullijos.rooms."
#define DIRECTORY_PERMISSIONS 0755
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))
#define NAME_LENGTH 8
#define TYPE_LENGTH 11
#define NUMBER_OF_ROOMS 7
#define MAX_CONNECTIONS (NUMBER_OF_ROOMS - 1)
#define MIN_CONNECTIONS 3

// Possible Room Names
static const char* ROOM_NAMES[] = {"GYGAX", "ARNESON", "KUNTZ", "BLUME", "WARD",
				   "HOLMES", "MOLDVAY", "MENTZER", "DENNING", "COOK"};
// Room struct
struct Room {
	char name[NAME_LENGTH];
	int numberOfConnections;
	struct Room* connections[MAX_CONNECTIONS];
	char type[TYPE_LENGTH];
};

// Function Prototypes
void makeDirectory(char* stringBuffer);
void appendProcessIDToDirectoryNamePrefix(char* stringBuffer);
void makeRooms(struct Room* roomArray);
void initializeRooms(struct Room* roomArray);
void setRoomNames(struct Room* roomArray);
void selectRandomName(char* nameBuffer);
int getRandomIntInRange(int minInt, int MaxInt);
int isNameTaken(char* nameBuffer, struct Room* roomArray);
void setConnections(struct Room* roomArray);
int hasMinConnections(struct Room* room);
struct Room* getRandomRoom(struct Room* roomArray);
int isValidConnection(struct Room* room1, struct Room* room2);
int areConnected(struct Room* room1, struct Room* room2);
int isSameRoom(struct Room* room1, struct Room* room2);
int hasMaxConnections(struct Room* room);
void connectRooms(struct Room* room1, struct Room* room2);
void setRandomRoomType(struct Room* roomArray, char* type);
void setRemainingRoomsType(struct Room* roomArray, char* type);
int hasType(struct Room* room);
void makeRoomFiles(struct Room* roomArray, char* directoryName);
void writeRoomDataToFile(struct Room* room, char* filePathBuffer);
int hasConnection(struct Room* room, int index);

int main() {
	// Seed rand()
	srand(time(NULL));

	// Make a directory
	char directoryName[STRING_BUFFER_SIZE];
	memset(directoryName, '\0', STRING_BUFFER_SIZE);
	makeDirectory(directoryName);

	// Make rooms
	struct Room roomArray[NUMBER_OF_ROOMS];
	makeRooms(roomArray);

	// Write rooms to files in directory
	makeRoomFiles(roomArray, directoryName);

	return 0;
}

/********** FUNCTIONS FOR MAKING DIRECTORY **********/

// Make directory name and create directory
void makeDirectory(char* stringBuffer) {
	appendProcessIDToDirectoryNamePrefix(stringBuffer);
	mkdir(stringBuffer, DIRECTORY_PERMISSIONS);
}

// Get process ID and append o direcotry name prefix
void appendProcessIDToDirectoryNamePrefix(char* stringBuffer) {
	int processID = getpid();
	sprintf(stringBuffer, DIRECTORY_NAME_PREFIX"%d", processID);
}



/********** FUNCTIONS FOR MAKING ROOM FILES **********/

void makeRooms(struct Room* roomArray) {
	// clear memory of room data members
	initializeRooms(roomArray);

	// Set names
	setRoomNames(roomArray);

	// Set connections
	setConnections(roomArray);

	// Set room types
	setRandomRoomType(roomArray, "START_ROOM");
	setRandomRoomType(roomArray, "END_ROOM");
	setRemainingRoomsType(roomArray, "MID_ROOM");
}

void initializeRooms(struct Room* roomArray) {
	// iterate through array of rooms
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {

		// Clear memory for names
		memset(roomArray[i].name, '\0', NAME_LENGTH);

		// Set number of connections to 0
		roomArray[i].numberOfConnections = 0;

		// iterate through connection pointers and set them to NULL
		int j;
		for (j = 0; j < MAX_CONNECTIONS; ++j) {
			roomArray[i].connections[j] = NULL;
		}

		// Clear memory for types
		memset(roomArray[i].type, '\0', TYPE_LENGTH);
	}
}



/***** FUNCTIONS FOR SETTING ROOM NAMES *****/

void setRoomNames(struct Room* roomArray) {
	// Create buffer for holding random name and get a random name for each room
	int i;
	char nameBuffer[NAME_LENGTH];
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {

		// Select a random name until one that hasn't been used is selected
		do {
			memset(nameBuffer, '\0', NAME_LENGTH);
			selectRandomName(nameBuffer);
		} while (isNameTaken(nameBuffer, roomArray));

		// Assign the name to the room
		strcpy(roomArray[i].name, nameBuffer);
	}
}

void selectRandomName(char* nameBuffer) {
	// Get number of names in room names array
	int arrayLength = ARRAY_LENGTH(ROOM_NAMES);

	// Choose a random index from the room names array
	int randomIndex = getRandomIntInRange(0, arrayLength - 1);

	// copy name at random index to string buffer passed in
	strcpy(nameBuffer, ROOM_NAMES[randomIndex]);
}

// Get random number in range from minInt through maxInt
int getRandomIntInRange(int minInt, int maxInt) {
	return (rand() % (maxInt + 1 - minInt) + minInt);
}

// Check if another room has this name passed in
int isNameTaken(char* nameBuffer, struct Room* roomArray) {
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {
		if (strcmp(roomArray[i].name, nameBuffer) == 0) {
			return 1;
		}
	}
	return 0;
}


/***** FUNCTIONS FOR CONNECTING ROOMS *****/

void setConnections(struct Room* roomArray) {
	// Iterate through rooms
	int i;
	struct Room* randomRoom = NULL;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {

		// While room has fewer than the minimum connections
		while (!hasMinConnections(&roomArray[i])) {

			// Select rooms randomly until a valid connection can be made
			do {
				randomRoom = getRandomRoom(roomArray);
			}while (!isValidConnection(&roomArray[i], randomRoom));

			// Connect room with randomly chosen room
			connectRooms(&roomArray[i], randomRoom);
		}
	}
}

// Check if room has minimum number of connections
int hasMinConnections(struct Room* room) {
	if(room->numberOfConnections >= MIN_CONNECTIONS) {
		return 1;
	}
	return 0;
}

// Return pointer to a random room from the room array
struct Room* getRandomRoom(struct Room* roomArray) {
	int randomIndex = getRandomIntInRange(0, NUMBER_OF_ROOMS - 1);
	return &roomArray[randomIndex];
}

// Check if rooms can be legally connected
int isValidConnection(struct Room* room1, struct Room* room2) {
	if (areConnected(room1, room2) || // are rooms already connected?
	      isSameRoom(room1, room2) || // do pointers point to same room?
	      hasMaxConnections(room1) || // does either room have max connections?
	      hasMaxConnections(room2)) {
		return 0;
	}
	return 1;
}

// Check if rooms are already connected
int areConnected(struct Room* room1, struct Room* room2) {
	int i;
	for (i = 0; i < room1->numberOfConnections; ++i) {
		if (isSameRoom(room1->connections[i], room2)) {
			return 1;
		}
	}
	return 0;
}

// Check if rooms are the same room
int isSameRoom(struct Room* room1, struct Room* room2) {
	if (strcmp(room1->name, room2->name) == 0) {
		return 1;
	}
	return 0;
}

// Check if room has maximum allowed connections
int hasMaxConnections(struct Room* room) {
	if(room->numberOfConnections == MAX_CONNECTIONS) {
		return 1;
	}
	return 0;
}

// Connect rooms
void connectRooms(struct Room* room1, struct Room* room2) {
	// connect room2 to room1 and increment number of connections
	room1->connections[room1->numberOfConnections] = room2;
	room1->numberOfConnections++;

	// connect room1 to room2 and increment number of connections
	room2->connections[room2->numberOfConnections] = room1;
	room2->numberOfConnections++;
}



/***** FUNCTIONS FOR SETTING ROOM TYPES *****/

// Get a random room and set its type to type passed into function
void setRandomRoomType(struct Room* roomArray, char* type) {

	// Select random rooms until one without a type is chosen
	struct Room* randomRoom = NULL;
	do {
		randomRoom = getRandomRoom(roomArray);
	}while (hasType(randomRoom));

	// Assign type passed to function to random room
	strcpy(randomRoom->type, type);
}

// Set all rooms that dont already have a type to the type passed to function
void setRemainingRoomsType(struct Room* roomArray, char* type) {
	// Iterate through rooms
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {

		// If room has doesn't have type, set type
		if (!hasType(&roomArray[i])) {
			strcpy(roomArray[i].type, type);
		}
	}
}

// Check if room has type
int hasType(struct Room* room) {
	if (room->type[0] != '\0') {
		return 1;
	}
	return 0;
}


/********** FUNCTIONS FOR MAKING ROOM FILES **********/


void makeRoomFiles(struct Room* roomArray, char* directoryName) {
	// Set length of file path buffer and declare buffer
	int filePathLength = STRING_BUFFER_SIZE + NAME_LENGTH;
	char filePathBuffer[filePathLength];

	// iterate through rooms
	int i;
	for (i = 0; i < NUMBER_OF_ROOMS; ++i) {

		// clear buffer, then assemble the file path
		memset(filePathBuffer, '\0', filePathLength);
		sprintf(filePathBuffer, "%s/%s", directoryName, (&roomArray[i])->name);

		// Write the rooms data to file
		writeRoomDataToFile(&roomArray[i], filePathBuffer);
	}
}

// Write room data to file
void writeRoomDataToFile(struct Room* room, char* filePathBuffer) {
	// declare and open file
	FILE* file = fopen(filePathBuffer, "w");

	// write room name to file
	fprintf(file, "ROOM NAME: %s\n", room->name);

	// iterate through connections
	int i;
	int connectionNumber = 1;
	for (i = 0; i < MAX_CONNECTIONS; ++i) {

		// if connection is not NULL, write connection to file and increment connection number
		if (hasConnection(room, i)) {
			fprintf(file, "CONNECTION %d: %s\n", connectionNumber, room->connections[i]->name);
			connectionNumber++;
		}
	}

	// write room type to file
	fprintf(file, "ROOM TYPE: %s\n", room->type);

	// close file
	fclose(file);
}

// Check if connection points to NULL or room
int hasConnection(struct Room* room, int index) {
	if (room->connections[index] != NULL) {
		return 1;
	}
	return 0;
}
