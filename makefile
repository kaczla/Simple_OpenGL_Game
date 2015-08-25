SOURCE_DIR = ./src/
SOURCE = camera.o shader.o model.o light.o
MAIN = $(SOURCE_DIR)main.cpp
CXXFLAGS = -std=c++11
LFLAGS = -lSDL2 -lGL -lGLU -lGLEW -lIL -lILU -lassimp
APP_NAME = game.app

.PHONY: all clean
.DELETE_ON_ERROR: clean

all: pre_build main_build post_build
pre_build:
	@echo '*** START BUILDING ***'
	@echo ' '
post_build:
	@echo '*** FINISH BUILDING ***'
main_build: $(SOURCE)
	@echo ' '
	@echo 'Building application $(APP_NAME)'
	$(CXX) $(CXXFLAGS) $(MAIN) $(SOURCE) -o $(APP_NAME) $(LFLAGS)
	@echo 'Finished building application $(APP_NAME)'
	@echo ' '
	@echo ' '
	@echo 'Cleaning'
	$(RM) *.o
	@echo 'Cleaned'
	@echo ' '

%.o: $(SOURCE_DIR)%.cpp
	@echo ' '
	@echo 'Building file $@ from $<'
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo 'Finished building file $<'
	@echo ' '

clear: clean
clean:
	echo ' '
	@echo 'Cleaning'
	$(RM) *.o
	$(RM) $(APP_NAME)
	@echo 'Cleaned'
	@echo ' '
