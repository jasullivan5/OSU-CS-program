CC=gcc
HEADERS=StringArray.h IntArray.h
SOURCES=smallsh.c StringArray.c IntArray.c
EXECUTABLE=smallsh

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) -o $@

clean:
	rm -f $(EXECUTABLE)
