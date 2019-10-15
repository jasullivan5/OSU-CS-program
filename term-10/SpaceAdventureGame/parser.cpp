#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "parser.h"

// Constructor just initializes token index.
Parser::Parser() {
	currentTokenIndex = 0;
}

// Get the lists of verb and noun files.
int Parser::loadFiles(std::string verbFileList, std::string nounFileList) {
	// Make sure that two separate files are being used for verb and noun files.
	if (verbFileList == nounFileList) {
		std::cerr << "Verb file list cannot be same file as noun file list.";
		return EXIT_FAILURE;
	}
	// Load verb files into memory.
	if (loadFileList(verbFileList, verbLists) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	// Load noun files into memory.
	if (loadFileList(nounFileList, nounLists) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void Parser::eraseSubString(std::string& str, std::string strToErase) {
	int position = std::string::npos;
	while ((position = str.find(strToErase)) != std::string::npos) {
		str.erase(position, strToErase.length());
	}
}

// Iterate through file lists, loading the word lists from each into a vector.
int Parser::loadFileList(std::string fileListName, std::vector< std::vector<std::string> > &wordLists) {
	// Remove any carriage return characters
	eraseSubString(fileListName, "\r");
	// Open file list
	std::ifstream file(fileListName.c_str());
	if (!file) {
		std::cerr << "Failed to open file: " << fileListName;
		return EXIT_FAILURE;
	}

	// Get directory path. All word files need to be in same directory as file list files.
	std::string directory = getDirectory(fileListName);

	// Read each file name from the list file.
	std::string fileName;
	while (getline(file, fileName)) {
		// Create path to file as list file only contains file name.
		fileName = directory + fileName;
		// Create new word list and fill it with words from current word file.
		std::vector<std::string> newList;
		if (loadFile(fileName, newList) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		// Push new word list to lists vector
		wordLists.push_back(newList);
	}
	file.close();
	return EXIT_SUCCESS;
}

// Returns directory-only substring of file path.
std::string Parser::getDirectory(std::string filePath) {
	int position = filePath.find_last_of("/\\");
	return filePath.substr(0, position + 1);
}

// Loop through word lists, loading words to word vectors
int Parser::loadFile(std::string fileName, std::vector<std::string> &wordList) {
	// Open word file.
	eraseSubString(fileName, "\r");
	std::ifstream file(fileName.c_str());
	if (!file) {
		std::cerr << "Failed to open file: " << fileName;
		return EXIT_FAILURE;
	}
	// Push words in file to word vectors
	std::string word;
    while (getline(file, word)) {
		eraseSubString(word, "\r");
        wordList.push_back(word);
    }
    file.close();
	return EXIT_SUCCESS;
}

// Get user input, format it, and compare to valid commands.
void Parser::processInput(std::string i) {
	// Reset member variables.
	inputTokens.clear();
	verb.clear();
	nouns.clear();
	currentTokenIndex = 0;

	// Process new input.
	input = i; // Set input member.
    inputToLowerCase(); // Set all input characters to lowercase.
	removeInvalidChars(); // Erase all non-alphanumeric non-space characters in input
    tokenizeInput(); // Load each input word as its own string into token vector.
	matchVerb(); // Compare word tokens to words in verb lists.
	// Compare remaining word tokens to words in noun lists.
	if (currentTokenIndex < inputTokens.size()) {
		matchNouns();
	}
	duplicateNounCheck(); // If the same noun was recorded twice, remove the second instance from the noun vector.
}

// Set all input characters to lowercase.
void Parser::inputToLowerCase() {
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
}

// Load each input word as its own string into token vector.
void Parser::tokenizeInput() {
	// Make string into i/o stream.
    std::stringstream inputStringStream(input);
	// Read each word from string and store it in vector.
    std::string temp;
    while (std::getline(inputStringStream, temp, ' ')) {
        inputTokens.push_back(temp);
    }
}

// Erase all non-alphanumeric non-space characters in input
void Parser::removeInvalidChars() {
	// For each char of input
	for (int i = 0; i < input.size(); i++) {
		// Check if it is non-alphanumeric or non-space
		if (input[i] != ' ' &&
			input[i] != '_' &&
			(input[i] < 'a' || input[i] > 'z') &&
			(input[i] < '0' || input[i] > '9'))
		{
			// Erase invalid char
			input.erase(i, 1);
			i--; // backup one index as uninspected char has shifted down.
		}
	}
}

// Compare word tokens to words in verb lists.
void Parser::matchVerb() {
	// Loop through input word tokens.
    for (int i = 0; i < inputTokens.size(); i++) {
        currentTokenIndex++; // Track the current token index.
		if (inputTokens[i].compare("look") == 0 && i + 1 < inputTokens.size() && inputTokens[i + 1].compare("at") == 0) {
			verb = "look at";
			return;
		}
		// Compare token to words from each verb list.
        for (std::vector<std::string> list : verbLists) {
			// If a match is found, set first word in file as command verb and exit loops.
            if (searchList(inputTokens[i], list)) {
                verb = list[0];
                return;
            }
        }
    }
}

// Compare remaining word tokens to words in noun lists.
void Parser::matchNouns() {
	// Loop through input tokens, from where matchVerb() left off.
	for (int i = currentTokenIndex; i < inputTokens.size(); i++) {
		// Compare token to words from each noun list.
		for (std::vector<std::string> list : nounLists) {
			// If match is found, push first word in file to noun vector for maximum of 2 nouns.
			if (searchList(inputTokens[i], list)) {
				nouns.push_back(list[0]);
				if (nouns.size() == 2) {
					return;
				}
			}
		}
	}
}

// Search for word token in file
bool Parser::searchList(std::string token, std::vector<std::string> &list) {
	/*Start search at second word in file as first word represents the action that will be selected if any match is found in file.
	  lower_bound() performs binary search for first value that is not less than token. This requires that word files be sorted alphabetically.
	  If no values less than token are found, it returns pointer to memory one past the end of the vector.
	  list.begin() is subtracted to get index of search result. as lower_bound returns iterator that points to resulting value.*/
    int index = lower_bound(std::next(list.begin(), 1), list.end(), token) - list.begin();

	// If resulting index is within bounds of list and element at index matches token, return true
    if (index < list.size() && list[index] == token) {
        return true;
    }
    return false;
}

// If the same noun was recorded twice, remove the second instance from the noun vector.
void Parser::duplicateNounCheck() {
	if (nouns.size() > 1 && nouns[0] == nouns[1]) {
		nouns.pop_back();
	}
}

// Getter for the verb member.
std::string Parser::getVerb() {
    return verb;
}

// Getter for the noun vector member.
const std::vector<std::string>& Parser::getNouns() const {
    return nouns;
}
