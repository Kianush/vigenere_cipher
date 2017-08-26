CC=g++
CFLAGS=-c -Wall
LDFLAGS=
HEADERS=vigenere.h
SOURCES=main.c vigenere.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=vigenere_cipher

all: $(HEADERS) $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o vigenere_cipher

