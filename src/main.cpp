#include <iostream>
#include <cstdlib>
#include <fstream>
//OpenGL:
#include <GL/glew.h>
//SDL:
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//CaughtSignal:
#include <signal.h>

using namespace std;

#define MIN_WindowWidth 640
#define MIN_WindowHeight 480

void CaughtSignal( int signal );

class Game{
public:
   Game();
   ~Game();
   void Start();
   void Update();
   void Stop();
   friend void GlobalStop( Game game );
private:
   //Init:
   void LoadSettings();
   void InitSDL();
   void InitOpenGL();
   void InitWindow();
   void InitContent();
   void SetIcon();
   SDL_Window *Window = NULL;
   bool CheckInit = true;
   bool FullScreen = false;
   string Title = "Simple OpenGL Game";
   int WindowPositionX = SDL_WINDOWPOS_CENTERED;
   int WindowPositionY = SDL_WINDOWPOS_CENTERED;
   int WindowWidth = MIN_WindowWidth;
   int WindowHeight = MIN_WindowHeight;
   bool WindowResizable = false;
   bool WindowBorderless = false;
   Uint32 WindowFlag = SDL_WINDOW_OPENGL;
   SDL_GLContext WindowGLContext = NULL;
   //Game:
   bool Exit = true;
   SDL_Event Event;
   //Other:
   fstream SettingsFile;
};

Game * PointerGame = NULL;
void GlobalStop();

int main( int argc, char* argv[] ){
   signal( SIGTSTP, CaughtSignal );
   signal( SIGINT, CaughtSignal );
   Game game;
   PointerGame = &game;
   game.Start();
   PointerGame = NULL;
   return 0;
}

void CaughtSignal( int signal ){
   //CTRL+Z
   if( signal == SIGTSTP ){
      cout<<"\n\nCaught signal SIGTSTP\n\n";
      GlobalStop();
   }
   //CTRL+C
   else if( signal == SIGINT ){
      cout<<"\n\nCaught signal SIGINT\n\n";
      GlobalStop();
   }
   //OTHER
   else{
      cout<<"\n\nCaught signal number: "<<signal<<"\n\n";
   }
}

void GlobalStop(){
   cout<<"GLOBAL STOP GAME\n";
   if( PointerGame == NULL ){
      cout<<"CAN'T FIND GAME!!!\n";
   }
   else{
      PointerGame->Stop();
   }
}

Game::Game(){
   this->LoadSettings();
   SDL_Log( "Constructor: INITIALIZE\n" );
   this->InitSDL();
   this->InitOpenGL();
   this->InitWindow();
   this->SetIcon();
   this->InitContent();
   this->Exit = this->CheckInit;
}

Game::~Game(){
   SDL_Log( "Destructor: CLEANING\n" );
   SDL_GL_DeleteContext( this->WindowGLContext );
   SDL_DestroyWindow( this->Window );
   SDL_Quit();
   //Save settings
   this->SettingsFile.open( "settings.init", ios::out );
   if( this->SettingsFile.good() ){
      this->SettingsFile<<"fullscreen "<<this->FullScreen
      <<"\nwidth "<<this->WindowWidth
      <<"\nheight "<<this->WindowHeight
      <<"\npositionx ";
      if( this->WindowPositionX == SDL_WINDOWPOS_CENTERED ){
         this->SettingsFile<<-1;
      }
      else{
         this->SettingsFile<<this->WindowPositionX;
      }
      this->SettingsFile<<"\npositiony ";
      if( this->WindowPositionY == SDL_WINDOWPOS_CENTERED ){
         this->SettingsFile<<-1;
      }
      else{
         this->SettingsFile<<this->WindowPositionY;
      }
      this->SettingsFile<<"\nborderless "<<this->WindowBorderless
      <<"\nresizable "<<this->WindowResizable;
      this->SettingsFile.close();
   }
}

void Game::LoadSettings(){
   this->SettingsFile.open( "settings.init", ios::in );
   if( this->SettingsFile.good() ){
      cout<<"Open file: settings.init\n";
      string InputString;
      int InputInt;
      while( this->SettingsFile>>InputString ){
         this->SettingsFile>>InputInt;
         if( InputString == "fullscreen" ){
            if( InputInt == 1 ){
               this->FullScreen = true;
               this->WindowFlag += SDL_WINDOW_FULLSCREEN;
            }
            else{
               this->FullScreen = false;
            }
         }
         else if( InputString == "width" ){
            if( InputInt >= MIN_WindowWidth ){
               this->WindowWidth = InputInt;
            }
         }
         else if( InputString == "height" ){
            if( InputInt >= MIN_WindowHeight ){
               this->WindowHeight = InputInt;
            }
         }
         else if( InputString == "positionx" ){
            if( InputInt < 0 ){
               this->WindowPositionX = SDL_WINDOWPOS_CENTERED;
            }
            else{
               this->WindowPositionX = InputInt;
            }
         }
         else if( InputString == "positiony" ){
            if( InputInt < 0 ){
               this->WindowPositionY = SDL_WINDOWPOS_CENTERED;
            }
            else{
               this->WindowPositionY = InputInt;
            }
         }
         else if( InputString == "borderless" ){
            if( InputInt == 1 ){
               this->WindowBorderless = true;
               this->WindowFlag += SDL_WINDOW_BORDERLESS;
            }
            else{
               this->WindowBorderless = false;
            }
         }
         else if( InputString == "resizable" ){
            if( InputInt == 1 ){
               this->WindowResizable = true;
               this->WindowFlag += SDL_WINDOW_RESIZABLE;
            }
            else{
               this->WindowResizable = false;
            }
         }
         else{
            cout<<"Unknown input: \""<<InputString<<"\" from file: settings.init\n";
         }
      }
      this->SettingsFile.close();
   }
   else{
      cout<<"Can't find file: settings.init\n";
   }
}

void Game::Start(){
   SDL_Log( "Game: BEGIN\n" );
   while( this->Exit ){
      while( SDL_PollEvent( &this->Event ) ){
         switch( this->Event.type ){
            case SDL_QUIT:
               this->Exit = false;
               SDL_Log( "Window closed\n" );
               break;
            case SDL_MOUSEMOTION:

               break;
            case SDL_KEYDOWN:
               switch( this->Event.key.keysym.sym ){
                  case SDLK_ESCAPE:
                     this->Exit = false;
                     SDL_Log( "Window closed\n" );
                     break;
                  default:
                     break;
               }
               break;
            case SDL_WINDOWEVENT:
               switch( this->Event.window.event ){
                  case SDL_WINDOWEVENT_MOVED:
                     SDL_Log( "Window moved to %d;%d\n",
                        this->Event.window.data1,
                        this->Event.window.data2
                     );
                     break;
                  case SDL_WINDOWEVENT_RESIZED:
                     SDL_Log( "Window resized to %dx%d\n",
                        this->Event.window.data1,
                        this->Event.window.data2
                     );
                     break;
                  case SDL_WINDOWEVENT_MINIMIZED:
                     SDL_Log( "Window minimized\n" );
                     break;
                  case SDL_WINDOWEVENT_MAXIMIZED:
                     SDL_Log( "Window maximized\n" );
                     break;
                  case SDL_WINDOWEVENT_RESTORED:
                     SDL_Log( "Window restored\n" );
                     break;
                  default:
                     break;
               }
               break;
            default:
               break;
         }
      }
      this->Update();
   }
   SDL_Log( "Game: END\n" );
}

void Game::Update(){
   glClear( GL_COLOR_BUFFER_BIT );

   SDL_GL_SwapWindow( this->Window );
}

void Game::InitSDL(){
   if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
      SDL_Log( "SDL_Init: %s\n", SDL_GetError() );
      this->CheckInit = false;
   }
   else{
      SDL_Log( "SDL_Init: SUCCESS\n" );
      //Check max resolution:

   }
}

void Game::Stop(){
   this->Exit = false;
   this->CheckInit = false;
   SDL_Log( "STOP GAME!\n" );
}

void Game::InitOpenGL(){
   if( this->CheckInit ){
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
      glewExperimental = true;
   }
}

void Game::InitWindow(){
   if( this->CheckInit ){
      this->Window = SDL_CreateWindow(
         this->Title.c_str(),
         this->WindowPositionX,
         this->WindowPositionY,
         this->WindowWidth,
         this->WindowHeight,
         this->WindowFlag
      );
      if( this->Window == NULL ){
         SDL_Log( "SDL_CreateWindow: %s\n", SDL_GetError() );
         this->CheckInit = false;
      }
      else{
         SDL_Log( "SDL_CreateWindow: SUCCESS\n" );
      }
   }
}

void Game::SetIcon(){
   if( this->CheckInit ){
      SDL_Surface *surface = NULL;
      Uint16 pixels[16*16] = {
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xffff, 0xffff, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xffff, 0xffff, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xffff, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000,
         0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000, 0xf000
      };
      surface = SDL_CreateRGBSurfaceFrom( pixels, 16, 16, 16, 16*2, 0x0f00, 0x00f0, 0x000f, 0xf000 );
      if( surface == NULL ){
         SDL_Log( "SDL_SetWindowIcon: %s\n", SDL_GetError() );
      }
      else{
         SDL_Log( "SDL_SetWindowIcon: SUCCESS\n" );
         SDL_SetWindowIcon( this->Window, surface );
         SDL_FreeSurface( surface );
      }
   }
}

void Game::InitContent(){
   if( this->CheckInit ){
      this->WindowGLContext = SDL_GL_CreateContext( this->Window );
      if( this->WindowGLContext == NULL ){
         SDL_Log( "SDL_GL_CreateContext: %s\n", SDL_GetError() );
         this->CheckInit = false;
         return;
      }
      SDL_Log( "SDL_GL_CreateContext: SUCCESS\n" );
      glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
      glClear( GL_COLOR_BUFFER_BIT );
      SDL_GL_SwapWindow( this->Window );
      SDL_Log( "OpenGL version: %s", glGetString( GL_VERSION ) );
   }
}
