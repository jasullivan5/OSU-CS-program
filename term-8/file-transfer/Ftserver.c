/*********************************************************************
** Program Filename: Ftserver.c
** Author: Joseph Sullivan, ONID: sullijos
** Class: CS 372 Intro to Computer Networks
** Date last modified: 03/10/2019
** Tested on: flip3.engr.oregonstate.edu and flip2.engr.oregonstate.edu
** Description: Implements a file transfer server over a pair of TCP connections.
**              Sets up listening port for accepting client control connection
**              requests. Once control connection is established, command from
**              client is received. The server will request a data connection
**              from the client if the command received is valid. A list of files
**              in the current working directory will be sent to client if command
**              is "-l", a file's contents will be sent if the command is "-g <filename>".
**              If the command or file name requested are invalid, an error message is sent.
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>

#define NUM_ARGS 2
#define PORT 1
#define NUM_CONNECTIONS 10
#define BUFFER_LENGTH 255
#define COMMAND_LENGTH 2
#define LIST_COMMAND "-l"
#define GET_COMMAND "-g"
#define INVALID_COMMAND "Command was invalid. Valid commands:\n-l             --List directory\n-g <filename>  --Get file"
#define DIRECTORY_NAME "./"

/*********************************************************************
** Function: void error(char *message)
** Description: Calls perror and exits program with EXIT_FAILURE.
** Parameters: char *message: Error message passed to perror.
*********************************************************************/
void error(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}


/*********************************************************************
** Function: void validateArgs(int argc)
** Description: Ensures that the minimum required number of arguments
**              was passed to the main function.
** Parameters: int argc: number of command line arguments.
*********************************************************************/
void validateArgs(int argc)
{
    if(argc < NUM_ARGS)
    {
        fprintf(stderr, "USAGE: Ftserver port\n");
        exit(EXIT_FAILURE);
    }
}


/*********************************************************************
** Function: void setListeningPort(char *listeningPort, struct addrinfo **serverInfo)
** Description: Sets parameters for desired address family, socket type,
**              and address type. Requests info of host indicated by the
**              hostname and port indicated in command line arguments. Prints
**              error and exits upon failure to obtain info.
** Parameters: char *listeningPort: Port that server will listen for connections on.
**             struct addrinfo **serverInfo: struct to populate with requested address info.
*********************************************************************/
void setListeningPort(char *listeningPort, struct addrinfo **serverInfo)
{
    // Set parameters for host address and socket
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get server address info
    int result = getaddrinfo(NULL, listeningPort, &hints, serverInfo);

    // Print error and exit upon failure to obtain server info
    if (result != 0)
    {
        fprintf(stderr, "ERROR: Failed to obtain server info\n");
        freeaddrinfo(*serverInfo);
        exit(EXIT_FAILURE);
    }
}


/*********************************************************************
** Function: int getListeningSocket(struct addrinfo *serverInfo)
** Description: Attempts to creating socket and bind address to it with each
**              result stored in serverInfo's linked list. If all attempts fail,
**              an error is printed and program exits. Success returns socket
**              file descriptor that will be used to listen for client
**              connection requests.
** Parameters: struct addrinfo *serverInfo: struct containing linked list of
**             host address info results obtained in getServerInfo().
** Return: int socket file descriptor for listening for client connection requests.
*********************************************************************/
int getListeningSocket(struct addrinfo *serverInfo)
{
    struct addrinfo *addrinfoPtr = serverInfo;
    int socketFD;
    int bindResult;

    // Try all server address info results in linked list
    while (addrinfoPtr != NULL)
    {
        // Get socket file descriptor
        socketFD = socket(addrinfoPtr->ai_family, addrinfoPtr->ai_socktype, addrinfoPtr->ai_protocol);
        if (socketFD != -1)
        {
            // bind address to socket
            bindResult = bind(socketFD, addrinfoPtr->ai_addr, addrinfoPtr->ai_addrlen);
            if (bindResult != -1)
            {
                break; // Exit while loop if binding succeeds
            }
        }
        // If bind fails, close socket and try next address info result in list
        close(socketFD);
        addrinfoPtr = addrinfoPtr->ai_next;
    }
    freeaddrinfo(serverInfo); // Fee serverInfo once we are done with it

    // If all results fail print errror and exit
    if (addrinfoPtr == NULL)
    {
        error("ERROR: Failed to bind address to socket");
    }
    return socketFD; // Otherwise, return socket file descriptor for bound socket
}


/*********************************************************************
** Function: int acceptClient(int listeningSocket, char *clientName, size_t clientNameSize)
** Description: Accepts connection request from client and creates socket for connection.
**              If accept fails, program exits with error. Gets hostname of client and
**              returns socket file descriptor for control connection.
** Parameters: int listeningSocket: Socket that server is listening for client connection requests on.
**             char *clientName: String for storing host name of client.
**             size_t clientNameSize: Size of buffer for storing client host name.
** Return: int socket file descriptor for control connection to client.
*********************************************************************/
int acceptClient(int listeningSocket, char *clientName, size_t clientNameSize)
{
    struct sockaddr_storage clientInfo;
    socklen_t clientInfoSize = sizeof(clientInfo);

    // Accept connection request from client and create socket
    int controlSocket = accept(listeningSocket, (struct sockaddr *)&clientInfo, &clientInfoSize);

    // Print erro upon failure to accept connection
    if (controlSocket < 0)
    {
        error("ERROR: Failed to accept connection from client");
    }

    // Get hostname of client
    getnameinfo((struct sockaddr*)&clientInfo, sizeof(clientInfo), clientName, clientNameSize, NULL, 0, NI_NOFQDN);

    return controlSocket; // return socket file descriptor
}


/*********************************************************************
** Function: void getClientInfo(struct addrinfo **clientInfo, char *clientName, char *port)
** Description: Sets parameters for desired address family, socket type,
**              and address type. Requests info of host indicated by the
**              hostname and port where client will be listening for connection.
**              Prints error and exits upon failure to obtain info.
** Parameters: struct addrinfo **clientInfo: Struct for storing client info.
**             char *clientName: Host name of client.
**             char *port: Port that client will be listening on.
*********************************************************************/
void getClientInfo(struct addrinfo **clientInfo, char *clientName, char *port)
{
    // Set parameters for client address info retrieved by getaddrinfo()
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Get client info
    int result = getaddrinfo(clientName, port, &hints, clientInfo);

    // Print error and exit upon failure to obtain client info
    if (result != 0)
    {
        fprintf(stderr, "ERROR: Failed to obtain client info\n");
        freeaddrinfo(*clientInfo);
        exit(EXIT_FAILURE);
    }
}


/*********************************************************************
** Function: int connectToClient(struct addrinfo *clientInfo)
** Description: Tries connecting to client with each set of address info results
**              stored in clientInfo's linked list. If all connections fail, an
**              error is printed and program is exited. Success returns socket
**              file descriptor of connection.
** Parameters: struct addrinfo *clientInfo: struct containing linked list of
**             host address info results obtained in getClientInfo().
** Return: Returns int socket file descriptor of data connection to client.
*********************************************************************/
int connectToClient(struct addrinfo *clientInfo)
{
    struct addrinfo *addrinfoPtr = clientInfo;
    int socketFD;
    int connectResult;

    // Try all client address info results in linked list
    while (addrinfoPtr != NULL)
    {
        // Get socket file descriptor
        socketFD = socket(addrinfoPtr->ai_family, addrinfoPtr->ai_socktype, addrinfoPtr->ai_protocol);
        if (socketFD != -1)
        {
            // Connect with client
            connectResult = connect(socketFD, addrinfoPtr->ai_addr, addrinfoPtr->ai_addrlen);
            if (connectResult != -1)
            {
                break; // Exit while loop if connection succeeds
            }
        }
        // If connection fails, close socket and try next address info result in list
        close(socketFD);
        addrinfoPtr = addrinfoPtr->ai_next;
    }

    // Done with clientInfo
    freeaddrinfo(clientInfo);

    // If all results fail print errror and exit
    if (addrinfoPtr == NULL)
    {
        error("ERROR: Failed to connect to client");
    }
    return socketFD; // Otherwise, return socket file descriptor for connection
}


/*********************************************************************
** Function: int receiveMessage(int controlSocket, char *message)
** Description: Receives message from client until '\0' is recieved or buffer
**              is full. Exits with erro if recv() fails.
** Parameters: int controlSocket: Socket for file transfer control messages.
**             char *message: String for storing message received from client.
** Return: int result of last call to recv()
*********************************************************************/
int receiveMessage(int controlSocket, char *message)
{
    int charsReceived = 0;
    int totalCharsReceived = 0;
    int bufferLeft = BUFFER_LENGTH - 1; // -1 to ensure that message ends in '\0'

    // clear message buffer
    memset(message, '\0', BUFFER_LENGTH);

    do // Receive chars from client until '\0' is received or buffer is full
    {
        // Subtract number of chars received from remaining buffer space
        bufferLeft -= charsReceived;
        // Receive message from client
        charsReceived = recv(controlSocket, &message[totalCharsReceived], bufferLeft, 0);
        // Exit with error upon failure to receive message
        if (charsReceived < 0)
        {
            close(controlSocket);
            error("ERROR: Failed to receive from client");
        }
        // Add chars received to total
        totalCharsReceived += charsReceived;
    } while(message[totalCharsReceived] != '\0' && bufferLeft > 0);
    return charsReceived; // Return last result of call to recv()
}


/*********************************************************************
** Function: void parseMessage(char *message, char *clientPort, char *command, char *fileName)
** Description: Divide message from client into Strings for client listening port, command
**              and name of file requested.
** Parameters: char *message: Message receved from client.
**             char *clientPort: String for storing client listening port.
**             char *command: String for storing command.
**             char *fileName: String for storing name of file requested.
*********************************************************************/
void parseMessage(char *message, char *clientPort, char *command, char *fileName)
{
    // Get sub-string of message up to first space
    char *token;
    token = strtok(message, " ");

    // If not NULL, copy token to clientPort string and get next token
    if(token != NULL)
    {
        strcpy(clientPort, token);
        token = strtok(NULL, " ");
    }
    // If not NULL, copy token to command string and get next token
    if(token != NULL)
    {
        strcpy(command, token);
        token = strtok(NULL, " ");
    }
    // If not NULL, copy token to fileName string
    if(token != NULL)
    {
        strcpy(fileName, token);
    }
}


/*********************************************************************
** Function: void sendToClient(int socketFD, char *message, size_t messageLength)
** Description: Converts the message length to a string and sends that to client first.
**              Then sends message to client. Exits with error if send() fails.
** Parameters: int socketFD: Socket to send message on
**             char *message: Message to send to client
**             size_t messageLength: Length of message being sent
*********************************************************************/
void sendToClient(int socketFD, char *message, size_t messageLength)
{
    int charsSent = 0;
    int totalCharsSent = 0;

    //Convert message length from size_t to string
    char lengthString[BUFFER_LENGTH];
    sprintf(lengthString, "%zu", messageLength);

    // Send message length string to client, including '\0' at end.
    while (totalCharsSent < strlen(lengthString) + 1)
    {
        charsSent = send(socketFD, &lengthString[totalCharsSent], (strlen(lengthString) + 1) - totalCharsSent, 0);
        // If send fails, exit with error
        if (charsSent < 0)
        {
            error("ERROR: Failed to send to client");
        }
        // Add number of chars sent to total
        totalCharsSent += charsSent;
    }

    // reset variables for tracking how many chars were sent
    charsSent = 0;
    totalCharsSent = 0;

    // Send message to client
    while (totalCharsSent < messageLength)
    {
        charsSent = send(socketFD, &message[totalCharsSent], messageLength - totalCharsSent, 0);
        // If send fails, exit with error
        if (charsSent < 0)
        {
            error("ERROR: Failed to send to client");
        }
        // Add number of chars sent to total
        totalCharsSent += charsSent;
    }

}


/*********************************************************************
** Function: void sendDirectoryList(int dataSocket)
** Description: Counts the number of files in the current working directory
**              and sends them to the client over the data socket.
** Parameters: int dataSocket: Socket to send directory list over.
*********************************************************************/
void sendDirectoryList(int dataSocket)
{
    // Open directory
    DIR *directory = opendir(DIRECTORY_NAME);
    if (directory == NULL)
    {
        error("ERROR: Couldn't open directory");
    }

    // count files in directory
    int fileCount = 0;
    struct dirent *directoryEntry;
    while ((directoryEntry = readdir(directory)))
    {
        fileCount++;
    }

    // convert file count to string and send to client
    char fileCountString[BUFFER_LENGTH];
    sprintf(fileCountString, "%d", fileCount);
    sendToClient(dataSocket, fileCountString, strlen(fileCountString));

    // rewind directory and send each file name
    rewinddir(directory);
    while ((directoryEntry = readdir(directory)))
    {
        sendToClient(dataSocket, directoryEntry->d_name, strlen(directoryEntry->d_name));
    }
    closedir(directory);
}


/*********************************************************************
** Function: int validateFile(char *fileName)
** Description: Compares file name requested by client to those in current
**              working directory.
** Parameters: char *fileName: Name of file that client requested.
** Return: int 0 if file name was invalid, 1 if valid.
*********************************************************************/
int validateFile(char *fileName)
{
    // Open directory
    DIR *directory = opendir(DIRECTORY_NAME);
    if (directory == NULL)
    {
        error("ERROR: Couldn't open directory");
    }

    // iterate through files in directory
    struct dirent *directoryEntry;
    while ((directoryEntry = readdir(directory)))
    {
        // if file requested matches current entry return 1
        if (strcmp(fileName, directoryEntry->d_name) == 0)
        {
            closedir(directory);
            return 1;
        }
    }
    // If no file names matched the file requested, return 0
    closedir(directory);
    return 0;
}


/*********************************************************************
** Function: size_t fileToString(char *fileName, char **fileString)
** Description: Copies file contents to string.
** Parameters: char *fileName: File requested by client.
**             char **fileString: String to store file contents in.
** Return:     size_t length of file.
*********************************************************************/
size_t fileToString(char *fileName, char **fileString)
{
    //Open file
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        error("ERROR: Failed to open file");
    }

    // Go to end position of file and get file length
    fseek(file, 0, SEEK_END);
    size_t fileLength = ftell(file);

    // allocate memory for string
    *fileString = malloc(fileLength + 1);
    memset(*fileString, '\0', fileLength + 1);

    // Rewind file and read file to string
    rewind(file);
    fread(*fileString, sizeof(char), fileLength, file);
    fclose(file);
    return fileLength;
}


/*********************************************************************
** Function: void executeCommand(int controlSocket, char *clientName, char *clientPort, char *command, char *fileName)
** Description: If command is "-l", data connection is requested and list of files
**              in current working directory is sent to client. If command is "-g"
**              and file name requested is valid, data connection is requested,
**              requested file is stored in string and sent to client. If command
**              or file name are invalid, error message is sent to client.
** Parameters: int controlSocket: Socket for sending control communications.
**             char *clientName: Host name of client.
**             char *clientPort: Client port to request data connection on.
**             char *command: Command received from client.
**             char *fileName: File requested by client.
*********************************************************************/
void executeCommand(int controlSocket, char *clientName, char *clientPort, char *command, char *fileName)
{
    int dataSocket;
    struct addrinfo *clientInfo;

    if (strcmp(LIST_COMMAND, command) == 0) // Client requested directory list
    {
        // Let client know that directory list will be sent
        sendToClient(controlSocket, LIST_COMMAND, strlen(LIST_COMMAND));
        printf("List directory requested on client port %s\n", clientPort);

        // Request data connection
        getClientInfo(&clientInfo, clientName, clientPort);
        dataSocket = connectToClient(clientInfo);

        // Send directory list to client
        printf("Sending directory contents to %s:%s\n", clientName, clientPort);
        sendDirectoryList(dataSocket);
        close(dataSocket);
    }
    else if (strcmp(GET_COMMAND, command) == 0) // Client requested file
    {
        // if file name is valid
        if (validateFile(fileName))
        {
            // Let client know file will be sent
            sendToClient(controlSocket, GET_COMMAND, strlen(GET_COMMAND));
            printf("File %s requested on client port %s\n", fileName, clientPort);

            // Request data connection
            getClientInfo(&clientInfo, clientName, clientPort);
            dataSocket = connectToClient(clientInfo);

            // Store file in string
            char *fileString;
            size_t fileLength = fileToString(fileName, &fileString);

            // Send file string to client
            printf("Sending %s to %s:%s\n", fileName, clientName, clientPort);
            sendToClient(dataSocket, fileString, fileLength);
            close(dataSocket);
        }
        else // Invalid file name requested
        {
            // Send error message to client
            char errorMessage[BUFFER_LENGTH];
            printf("File %s requested on client port %s\n", fileName, clientPort);
            sprintf(errorMessage, "File %s not found", fileName);
            printf("%s\n", errorMessage);
            sendToClient(controlSocket, errorMessage, strlen(errorMessage));
        }
    }
    else // Invalid command from client
    {
        // send error message to client
        printf("Invalid command from client: %s\n", clientName);
        sendToClient(controlSocket, INVALID_COMMAND, strlen(INVALID_COMMAND));
    }
}


/*********************************************************************
** Function: int main(int argc, char *argv[])
** Description: Implements a file transfer server over a pair of TCP connections.
**              Sets up listening port for accepting client control connection
**              requests. Once control connection is established, command from
**              client is received. The server will request a data connection
**              from the client if the command received is valid. A list of files
**              in the current working directory will be sent to client if command
**              is "-l", a file's contents will be sent if the command is "-g <filename>".
**              If the command or file name requested are invalid, an error message is sent.
** Parameters: int argc: Number of command line arguments
**             char *argv[]: Array of command line arguments
** Return:     0 upon exit of program.

*********************************************************************/
int main(int argc, char *argv[])
{
    //Validate command line arguments
    validateArgs(argc);

    //Set listening port
    struct addrinfo *serverInfo;
    setListeningPort(argv[PORT], &serverInfo);

    //Bind address to socket and listen
    int listeningSocket = getListeningSocket(serverInfo);
    setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEPORT, (char *) 1, sizeof(int));
    int listenResult = listen(listeningSocket, NUM_CONNECTIONS);
    if (listenResult < 0)
    {
        close(listeningSocket);
        error("ERROR: Failed to listen on port");
    }
    printf("Server open on port %s\n", argv[PORT]);

    int controlSocket;
    int receiveResult;
    char clientName[BUFFER_LENGTH];
    char message[BUFFER_LENGTH];
    char clientPort[BUFFER_LENGTH];
    char command[BUFFER_LENGTH];
    char fileName[BUFFER_LENGTH];
    // Listen for client connection requests until SIGINT is recieved.
    while(1)
    {
        //Accept connection from client
        controlSocket = acceptClient(listeningSocket, clientName, BUFFER_LENGTH);
        printf("Connection from client: %s established\n", clientName);

        //Receive message from client
        receiveResult = receiveMessage(controlSocket, message);
        if (receiveResult == 0)
        {
            printf("Client closed control connection\n");
        }
        else
        {
            // Separate message into port, command, and file name strings
            parseMessage(message, clientPort, command, fileName);
            // Execute client's command and send reply
            executeCommand(controlSocket, clientName, clientPort, command, fileName);
        }
        close(controlSocket);
    }
    return 0;
}
