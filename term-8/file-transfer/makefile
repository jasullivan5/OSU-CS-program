all: Ftserver Ftclient.class

Ftclient.class: Ftclient.java
	javac Ftclient.java

Ftserver: Ftserver.c
	gcc -Wall -o Ftserver Ftserver.c

clean:
	rm -fr Ftserver Ftclient.class
