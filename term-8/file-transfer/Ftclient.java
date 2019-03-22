/*********************************************************************
** Program Filename: Ftclient.java
** Author: Joseph Sullivan, ONID: sullijos
** Class: CS 372 Intro to Computer Networks
** Date last modified: 03/10/2019
** Tested on: flip3.engr.oregonstate.edu and flip2.engr.oregonstate.edu
** Description: Implements a file transfer client.
                Requests control connection from server and sends command.
                Reads response from server over control connection.
                If command was valid, listens for data connection and receives data from server.
                "-l" command will request a list of files in the server's CWD and print them.
                "-g <filename>" will request the contents of file and create a copy of that file.
                If command is invalid an error message from the server is displayed.
*********************************************************************/
import java.net.*;
import java.io.*;

public class Ftclient
{
    private static final int SERVER_NAME = 0;
    private static final int SERVER_PORT = 1;
    private static final int COMMAND = 2;
    private static final int FILE_NAME = 3;
    private String serverName;
    private String serverPort;
    private String command;
    private String fileName;
    private String listeningPort;
    private String message;
    private Socket controlSocket;
    private Socket dataSocket;
    private ServerSocket listeningSocket;
    private BufferedReader reader;
    private PrintWriter writer;

    /*********************************************************************
    ** Function: Ftclient(String server, String servPort, String cmd, String file, String lisPort)
    ** Description: Requests control connection from server and sends command.
                    Reads response from server over control connection.
                    If command was valid, listens for data connection and receives data from server.
    ** Parameters: String server: server host name
                   String servPort: server listening port
                   String cmd: command to be sent to server
                   String file: name of file to be requested from server
                   String lisPort: client's listening port for data connection
    *********************************************************************/
    Ftclient(String server, String servPort, String cmd, String file, String lisPort)
    {
        serverName = server;
        serverPort = servPort;
        command = cmd;
        fileName = file;
        listeningPort = lisPort;
        try
        {
            // Connect to server
            controlSocket = new Socket(serverName, Integer.parseInt(serverPort));
            System.out.println("Connected to server: " + serverName);

            // Create reader and writer for socket communication
            reader = new BufferedReader(new InputStreamReader(controlSocket.getInputStream()));
            writer = new PrintWriter(controlSocket.getOutputStream(), true);

            // Set listening socket for data connection
            listeningSocket = new ServerSocket(Integer.parseInt(listeningPort));
            listeningSocket.setReuseAddress(true);

            // Send command
            sendCommand();

            // Read response from server
            readFromServer();

            // If command was valid, listen for data connection and receive data
            executeResponse();

            //cleanup
            controlSocket.close();
            listeningSocket.close();
        }
        catch(IOException i)
        {
            System.err.println(i);
        }
    }


    /*********************************************************************
    ** Function: public static Ftclient validateArgs(String args[])
    ** Description: Ensures the minimum number of arguments have been passed at program execution.
                    Assigns correct command line argument to Ftclient parameter based on number
                    of arguments when creating instance of Ftclient.
    ** Parameters: String args[]: Array of command line arguments.
    ** Return: New instance of Ftclient.
    *********************************************************************/
    public static Ftclient validateArgs(String args[])
    {
        // Ensures the minimum number of arguments have been passed at program execution
        if (args.length < 4 || ((args[COMMAND]).equals("-g") && args.length < 5))
        {
            System.err.println("USAGE: java Ftclient serverName serverPort command [fileName] dataPort");
            System.exit(1);
        }

        // If filename wasn't included in arguments, assign null to filename and data port is argument 4
        if (args.length < 5)
        {
            return new Ftclient(args[SERVER_NAME], args[SERVER_PORT], args[COMMAND], null, args[3]);
        }
        // If 5 arguments are passed, file name is argument 4 and data port is argument 5
        return new Ftclient(args[SERVER_NAME], args[SERVER_PORT], args[COMMAND], args[FILE_NAME], args[4]);
    }


    /*********************************************************************
    ** Function: private void sendCommand()
    ** Description: Sends the client's listening port, command, and, if applicable,
                    the requested file name to server over control connection.
    *********************************************************************/
    private void sendCommand()
    {
        // Combine listening port and command into one string
        String message = listeningPort + " " + command;

        // Adds requested file name to message if not null
        if (fileName != null)
        {
            message += " " + fileName;
        }

        // Send message to server
        writer.print(message);
        writer.flush();
    }


    /*********************************************************************
    ** Function: private void readFromServer()
    ** Description: Reads from server, first expecting message length string
                    that should always be '\0' terminated. Then, reads that number
                    of characters from socket which should be entire message.
    *********************************************************************/
    private void readFromServer()
    {
        try
        {
            int charRead;
            StringBuilder messageBuilder = new StringBuilder();

            do // read response length from server
            {
                charRead = reader.read();
                if (charRead == -1)
                {
                    System.err.println("Server closed connection");
                    System.exit(1);
                }

                // append characters to string builder
                messageBuilder.append((char)charRead);
            } while (charRead > 0);

            //Convert message length to string then to integer
            message = messageBuilder.toString();
            int length = Integer.parseInt(message.trim());

            //Reset string builder and read response from server
            messageBuilder.setLength(0);
            for (int i = 0; i < length; i++)
            {
                charRead = reader.read();
                if (charRead == -1)
                {
                    System.err.println("Server closed connection");
                    System.exit(1);
                }

                // append characters to string builder
                messageBuilder.append((char)charRead);
            }
            message = messageBuilder.toString();
        }
        catch(IOException i)
        {
            System.err.println(i);
        }
    }


    /*********************************************************************
    ** Function: private PrintWriter createFile()
    ** Description: If file alrady exists in local current working directory,
                    creates a PrintWriter for the file name with "_copy" appended
                    to file name. Attempts to place this before the file extension
                    if one exists. If the file doesn't already exist a PrintWriter
                    for the file is created.
    ** Return: new PrintWriter for appropriate file name
    *********************************************************************/
    private PrintWriter createFile()
    {
        try
        {
            // Check if file exists
            File file = new File(fileName);
            if (file.exists())
            {
                // If it does, find position of last '.' before extension
                int dotPosition = fileName.lastIndexOf('.');
                if (dotPosition == -1)
                {
                    // if no '.' in file name append "_copy" to name
                    return new PrintWriter(fileName + "_copy");
                }
                else
                {
                    // if file has extension, insert "_copy" before extension
                    return new PrintWriter(fileName.substring(0, dotPosition) + "_copy" + fileName.substring(dotPosition));
                }
            }
            // return file name if it doesn't exist
            return new PrintWriter(fileName);
        }
        catch(FileNotFoundException f)
        {
            System.err.println(f);
        }
        return null;
    }


    /*********************************************************************
    ** Function: private void executeResponse()
    ** Description: If command was valid, client listens for data connection request
                    from server. If server is sending directory list, first the number
                    of files in the server's directory is received, then each file name
                    is received and printed. If the server is sending a file, the file
                    contents are read from server, a file is created in the current
                    working directory, and the file contents are written to it. If the
                    command or file name sent was invalid, a error message from the server
                    is printed.
    *********************************************************************/
    private void executeResponse()
    {
        try
        {
            // If server confirms that directory list will be sent
            if ((message.trim()).equals("-l"))
            {
                // Listen for data connection and create reader for it.
                dataSocket = listeningSocket.accept();
                reader = new BufferedReader(new InputStreamReader(dataSocket.getInputStream()));

                // Get number of directory entries being sent from server
                System.out.println("Receiving directory structure from " + serverName + ":" + listeningPort);
                readFromServer();

                // Print each file in list as it is received
                int numFiles = Integer.parseInt(message.trim());
                for (int i = 0; i < numFiles; i++)
                {
                    readFromServer();
                    System.out.println(message);
                }
                dataSocket.close();
            }
            else if ((message.trim()).equals("-g"))
            {
                // Listen for data connection and create reader for it
                dataSocket = listeningSocket.accept();
                reader = new BufferedReader(new InputStreamReader(dataSocket.getInputStream()));

                // read file contents from server
                System.out.println("Receiving " + fileName + " from " + serverName + ":" + listeningPort);
                readFromServer();

                // Check if file already exists and either create it or create copy
                writer = createFile();

                // Write file contents from server to newly created file
                writer.write(message, 0, message.length());
                writer.close();
                System.out.println("File transfer complete.");
            }
            else
            {
                // Print error message from server if command or file was invalid
                System.err.println("Error from " + serverName + ":" + serverPort + ": " + message);
            }
        }
        catch(IOException i)
        {
            System.err.println(i);
        }
    }


    /*********************************************************************
    ** Function: public static void main(String args[])
    ** Description: Initiates program by calling validateArgs().
    ** Parameters: String args[]: Array of command line arguments.
    *********************************************************************/
    public static void main(String args[])
    {
        Ftclient client = Ftclient.validateArgs(args);
    }
}
