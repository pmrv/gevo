CPPFLAGS= -Wall -Werror -g `sdl2-config --cflags` -Og
LDLIBS=`sdl2-config --libs` -lstdc++

HEADERS=$(wildcard src/*.h)
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

TARGET = bin/gevo++

all: $(TARGET)

$(OBJECTS): $(SOURCES) $(HEADERS)

$(TARGET): $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(TARGET) $(OBJECTS) $(LDLIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
