all: Chatserve.class Chatclient

Chatserve.class: Chatserve.java
	javac Chatserve.java

Chatclient: Chatclient.c
	gcc -o Chatclient Chatclient.c

clean:
	rm -fr Chatserve.class Chatclient
