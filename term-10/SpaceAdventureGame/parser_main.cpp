#include <iostream>
#include "parser.hpp"

int main()
{
	// Get file list names
	std::string verbFileList;
	std::string nounFileList;
	std::cout << "Enter name of the verb file list: ";
	getline(std::cin, verbFileList);
	std::cout << "Enter name of the noun file list: ";
	getline(std::cin, nounFileList);

	// Initialize parser
	Parser parser;

	// Load Files
	int result = parser.loadFiles(verbFileList, nounFileList);
	if (result == EXIT_FAILURE) {
		return result;
	}

    // Get test input from user
	while (1) {
		std::string input;
		std::cout << "Enter a command: ";
		getline(std::cin, input);
		if (input == "quit") {
			return 0;
		}

		// Pass input to parser
		parser.processInput(input);

		// Print results
		std::cout << "parser.verb contains: ";
		std::cout << parser.getVerb();
		std::cout << std::endl;

		std::cout << "parser.nouns contains: ";
		for (std::string noun : parser.getNouns()) {
			std::cout << noun << " ";
		}
		std::cout << std::endl;
	}
    return 0;
}
