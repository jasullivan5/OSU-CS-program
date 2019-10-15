CC = g++ -std=c++11
CFLAGS = -Wall
OBJS = Item.o Area.o Player.o Exit.o Pickaxe.o parser.o Flashlight.o Mushroom.o Doohickey.o ScrewDriver.o PowerCrystal.o Spaceship.o data.o Alien.o Human.o Transmitter.o



spaceAdventure: spaceAdventure.cpp $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -g -o goAdventure spaceAdventure.cpp

classes.o: Item.o Area.o Player.o Exit.o parser.o data.o

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -g -c $< -o $@

.PHONY: clean
clean:
	rm *.o
