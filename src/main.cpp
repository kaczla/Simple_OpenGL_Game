#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std;

class Game{
public:
   Game();
   ~Game();
   void Start();
private:
   void Set();

   SDL_Window *Window = NULL;
   bool CheckInit = true;
};

int main( int argc, char* argv[] ){
   Game game;
   game.Start();
   return 0;
}

Game::Game(){

}

Game::~Game(){

}

void Game::Start(){

}
