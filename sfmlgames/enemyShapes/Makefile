CC=g++
CFLAGS=-c
LDFLAGS=-std=c++17
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SOURCES=src/main.cpp #Control.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
BINDIR=bin/

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Building..."
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BINDIR)$@ $(LIBFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LIBFLAGS)

clean:
	@echo "Removing game build..."
	rm *.o $(BINDIR)$(EXECUTABLE)

#install:
#	#install -s $(EXECUTABLE) $(BINDIR)
#	sudo cp -u $(EXECUTABLE) $(BINDIR)
#uninstall:
#	sudo rm $(BINDIR)/$(EXECUTABLE)