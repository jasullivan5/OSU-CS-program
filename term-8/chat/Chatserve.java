/*********************************************************************
** Program Filename: Chatserve.java
** Author: Joseph Sullivan, ONID: sullijos
** Class: CS 372 Intro to Computer Networks
** Date last modified: 02/10/2019
** Tested on: flip3.engr.oregonstate.edu
** Description: Implements a Chat server over a TCP connection.
**              Program accepts a port number argument from command line,
**              and listens on the port for a client connection. Upon
**              client request, program connects to client and then alternates
**              between recieving and sending messages from and to the client.
**              User can type \quit to close connection. Program continues to
**              listen on original port until SIGINT is received.
*********************************************************************/
import java.net.*;
import java.util.Scanner;
import java.io.*;

public class Chatserve
{
    public static final int MAX_MESSAGE_LENGTH = 500;
    public static final int PORT = 0;
    public int listeningPort;
    public ServerSocket listeningSocket;
    public Socket clientSocket;
    public String handle;
    public String message;
    public BufferedReader messageReader;
    public Scanner scanner;
    public PrintWriter messageWriter;

    /*********************************************************************
    ** Function: Chatserve(int port)
    ** Description: Initializes variables, listens for client connection,
    **              connects to client, then alternates between receiving
    **              and sending messages to and from client. Closes Streams
    **              and socket when connection is closed.
    ** Parameters: int port: port number for server to listen on.
    *********************************************************************/
    Chatserve(int port)
    {
        try
        {
            listeningPort = port;
            listeningSocket = null;
            clientSocket = null;
            handle = "Server> ";
            message = "";
            scanner = new Scanner(System.in);

            while(true) // Listen until SIGINT is recieved.
            {
                // Listen on port specified in command line argument
                listen();

                // Create reader and writer for socket
                messageReader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                messageWriter = new PrintWriter(clientSocket.getOutputStream(), true);

                do // Receive and send messages until message is null
                {
                    getClientMessage();
                    if (message != null)
                    {
                        sendMessage();
                    }
                }while (message != null);

                // Clear message, close client socket and streams
                message = "";
                messageReader.close();
                messageWriter.close();
                clientSocket.close();
                System.out.println("Streams and sockets closed");
            }
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
    }


    /*********************************************************************
    ** Function: public void listen()
    ** Description: Listens on port for client connection request.
    *********************************************************************/
    public void listen()
    {
        try
        {
            // Set port for listening socket if not done in previous call
            if (listeningSocket == null)
            {
                listeningSocket = new ServerSocket(listeningPort);
                listeningSocket.setReuseAddress(true);
            }
            System.out.println("Server is listening on port " + listeningPort);

            // Listen on socket
            clientSocket = listeningSocket.accept();
            System.out.println("Chat client connected");
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
    }


    /*********************************************************************
    ** Function: public void getClientMessage()
    ** Description: Reads message from client and prints it to stdout.
                    Alerts user that connection was lost if message is null.
    *********************************************************************/
    public void getClientMessage()
    {
        try
        {
            // read message from client
            message = messageReader.readLine();

            // if readLine() returns null, connection was closed or lost
            if (message == null)
            {
                System.out.println("Connection was lost");
            }
            else // else print received message
            {
                System.out.println(message);
            }
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
    }


    /*********************************************************************
    ** Function: public void sendMessage()
    ** Description: Gets message from server side user and sends it to client.
    **              Message is checked before sending to esnure that it is
    **              500 chars or less. If server user enters "\quit", message
    **              is set to null and function is exited.
    *********************************************************************/
    public void sendMessage()
    {
        do // Prompt for message to send until it is 500 chars or less
        {
            // Print handle and prompt for input
            System.out.print(handle);

            // Get input from server side user
            message = scanner.nextLine();

            // If server user quits, set message to null and return
            if (message.equals("\\quit") == true)
            {
                message = null;
                return;
            }

            // Alert user if message is too long
            if (message.length() > MAX_MESSAGE_LENGTH)
            {
                System.out.println("Message must be 500 or fewer characters");
            }
            else // Send handle and message if they are short enough
            {
                messageWriter.println(handle + message);
            }
        } while (message.length() > MAX_MESSAGE_LENGTH);
    }


    /*********************************************************************
    ** Function: public static void main(String args[])
    ** Description: Ensures that at least one argument was entered at
    **              command line for port number. Launches Chatserve, passing
    **              port number as argument.
    ** Parameters: String args[]: command lines argument for listening port number.
    *********************************************************************/
    public static void main(String args[])
    {
        // Ensure correct number of arguments
        if (args.length < 1)
        {
            System.err.println("USAGE: java Chatserve port");
            System.exit(1);
        }

        // Launch chat server with port number from command line arguments
        Chatserve server = new Chatserve(Integer.parseInt(args[PORT]));
    }
}
