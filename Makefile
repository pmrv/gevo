CPPFLAGS= -Wall -Werror -g `sdl2-config --cflags` -std=c++14 -O0 -pthread
LDLIBS=`sdl2-config --libs`

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
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(OBJECTS) $(LDLIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
