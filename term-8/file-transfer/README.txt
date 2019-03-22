Compile: Ftclient.java and Ftserver.c come with a make file for compilation.
           To compile both programs, just type "make" and hit enter.

Execute: Ftserver.c - Once compiled, you can run Ftserver.c by typing
         "Ftserver port". Replace 'port' with the port number that you want
         the file transfer server to listen for client connections on. Be sure to execute
         Ftserver before executing Ftclient.class.

        Ftclient.class - Once compiled, you can run Ftclient.class with one of two commands.

        To request a list of files in the server's current working directory, type
        "java Ftclient serverName serverPort -l dataPort". Replace 'serverName' with the name or IP
        address of the server you wish to connect to, replace 'serverPort' with the
        port number that the server is listening on, and replace 'dataPort' with the
        port number that you want the client to listen on for data connection requests from the server.

        To request that the contents of a file, type
        "java Ftclient serverName serverPort -g fileName dataPort".
        All command line arguments are the same, save for 'fileName', which should be
        replaced with the name of the file that you are requesting from the server.
