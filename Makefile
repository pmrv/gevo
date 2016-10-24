CPPFLAGS= -Wall -Werror -g `sdl2-config --cflags` -Og
LDLIBS=`sdl2-config --libs` -lstdc++

HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

TARGET = gevo++

all: $(TARGET)

$(OBJECTS): $(SOURCES) $(HEADERS)

$(TARGET): $(OBJECTS)

clean:
	rm -f $(TARGET) $(OBJECTS)
