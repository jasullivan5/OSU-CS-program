#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser
{
    private:
        // Private Member Variables
        std::vector< std::vector<std::string> > verbLists;
        std::vector< std::vector<std::string> > nounLists;
        std::string input;
        std::vector<std::string> inputTokens;
        int currentTokenIndex;
		std::string directory;
        std::string verb;
        std::vector<std::string> nouns;

        // Private Member Functions
        int loadFileList(std::string fileListName, std::vector< std::vector<std::string> > &wordLists);
        void eraseSubString(std::string& str, std::string strToErase);
        int loadFile(std::string fileName, std::vector<std::string> &wordList);
        void inputToLowerCase();
        void tokenizeInput();
		void removeInvalidChars();
        void matchVerb();
        void matchNouns();
        bool searchList(std::string token, std::vector<std::string> &list);
		void duplicateNounCheck();
		std::string getDirectory(std::string filePath);

    public:
        //Public Member Functions
        Parser();
		int loadFiles(std::string verbFileList, std::string nounFileList);
        void processInput(std::string i);
        std::string getVerb();
        const std::vector<std::string>& getNouns() const;
};

#endif // PARSER_H
