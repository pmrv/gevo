CPPFLAGS= -Wall -Werror -g `sdl2-config --cflags`
LDLIBS=`sdl2-config --libs` -lstdc++

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

TARGET = gevo++

all: $(TARGET)

$(OBJECTS): $(SOURCES)

$(TARGET): $(OBJECTS)

clean:
	rm -f $(TARGET) $(OBJECTS)
