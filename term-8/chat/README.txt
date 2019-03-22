Compile: Chatserve.java and Chatclient.c come with a make file for compilation.
           To compile both programs, just type "make" and hit enter.

Execute: Chatserve.java - Once compiled, you can run Chatserve.class by typing
         "java Chatserve port". Replace 'port' with the port number that you want
        the chat server to listen for client connections on. Be sure to execute
        Chatserve.class before executing Chatclient.

        Chatclient.c - Once compiled, you can run Chatclient by typing
        "Chatclient host_name port". Replace 'host_name' with the name or IP
        address of the server you wish to connect to, and replace 'port' with the
        port number that the server is listening on.

Control: Once both programs are executed, and the connection is successful,
         the client user will be prompted to enter their handle. Next, the client
         user will enter the first message. The server user will receive this message
         and then be prompted to reply. Each host alternates between sending and
         receiving messages until one user types "\quit". When either user types
         "\quit", the server will return to listening for a client connection request,
         and the client host will terminate the Chatclient program. The server user
         can stop listening and exit program by hitting ctrl-C to send a SIGINT. If
         one host is prompting for message input and the other terminates the connection,
         the host awaiting input will close the connection on their end once a message
         has been typed and the user has pressed enter.

Clean: Chatserve.class and Chatclient can be deleted by typing "make clean".
