CXX		:= g++
OUTPUT	:= sfmlgame

# compiler and linker flags
CXX_FLAGS := -O3 -std=c++17
INCLUDES  := -I./src
LDFLAGS   := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# the source files for the ecs game engine
SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

# all of these targets will be made if you type 'make'
all:$(OUTPUT)

# define the main executable requirements / command
$(OUTPUT):$(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

# specifies how the object files are compiled from cpp
.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

# typing 'make clean' will remove all intermediate build files
clean:
	rm -f $(OBJ_FILES) ./bin/sfmlgame

# typing 'make run' will compile and run the program
run: $(OUTPUT)
	cd bin && ./sfmlgame && cd ..