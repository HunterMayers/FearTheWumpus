CC = g++
CFLAGS = -g -Wall -pedantic -std=c++14 -O3

OSX = -I /Library/Frameworks/ -framework SDL2 -framework SDL2_image
LINUX = -lSDL2 -lSDL2_image

OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
HEADERS = $(wildcard *.h)

TARGET = AI

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: osx

run: all
	./$(TARGET)

osx: $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(OSX) -o $(TARGET)

linux: $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LINUX) -o $(TARGET)

clean:
	-rm -f *.o
	rm -f $(TARGET)
