
SOURCE_DIR = ./src
MAIN = $(SOURCE_DIR)/main.cpp
CXXFLAGS = -std=c++11
LFLAGS = -lSDL2 -lGL -lGLEW
APP_NAME = game.app

all:
	$(CXX) $(CXXFLAGS) $(MAIN) -o $(APP_NAME) $(LFLAGS)

clear:	clean
clean:
	rm *.o $(APP_NAME)
