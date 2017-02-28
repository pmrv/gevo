CPPFLAGS= -Wall -Werror -g `sdl2-config --cflags` -Og
LDLIBS=`sdl2-config --libs` -lstdc++ -pthread

HEADERS=$(wildcard src/*.h)
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

BUILDDIR=bin/

TARGET = $(BUILDDIR)/gevo++

all: dirs $(TARGET)

dirs:
	mkdir -p $(BUILDDIR)

$(OBJECTS): $(SOURCES) $(HEADERS)

$(TARGET): $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(TARGET) $(OBJECTS) $(LDLIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
