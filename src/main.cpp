#include <iostream>
#include <cstdlib>
#include <fstream>
//OpenGL:
#include <GL/glew.h>
//SDL:
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h> //SDL_SysWMinfo
#include <SDL2/SDL_opengl.h>
//glm:
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vec1.hpp>
//DevIL:
#include <IL/il.h>
#include <IL/ilu.h>
//CaughtSignal:
#include <signal.h>
//User include
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

using namespace std;

void CaughtSignal( int signal );

class Game{
public:
   Game();
   ~Game();
   void Start();
   void Stop();
private:
   //Init:
   void LoadSettings();
   void InitSDL();
   void InitOpenGL();
   void InitWindow();
   void InitContent();
   void InitDevIL();
   void SetIcon();
   //Min/Max resolution:
   int WindowMinWidth = 640;
   int WindowMinHeight = 480;
   int WindowMaxWidth = 640;
   int WindowMaxHeight = 480;
   //Window:
   SDL_Window *Window = NULL;
   bool CheckInit = true;
   bool FullScreen = false;
   string Title = "Simple OpenGL Game";
   int WindowPositionX = SDL_WINDOWPOS_CENTERED;
   int WindowPositionY = SDL_WINDOWPOS_CENTERED;
   int WindowWidth = 640;
   int WindowHeight = 480;
   int WindowWidthHalf = this->WindowWidth / 2;
   int WindowHeightHalf = this->WindowHeight / 2;
   bool WindowResizable = false;
   bool WindowBorderless = false;
   Uint32 WindowFlag = SDL_WINDOW_OPENGL;
   SDL_GLContext WindowGLContext = NULL;
   int WindowDisplayIndex = 0;
   SDL_DisplayMode CurrentDisplayMode;
   //Game:
   inline void Loop();
   inline void Update();
   bool Exit = true;
   SDL_Event Event;
   //Other:
   fstream SettingsFile;
   int i,j;
   GLenum GL_Error;
   ILenum IL_Error;
   //Camera:
   vec2 Mouse;
   Camera camera;
   //For change settings camera:
   vec1 Aspect;
   vec1 FOV;
   vec1 Near;
   vec1 Far;
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
   this->InitDevIL();
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
            if( InputInt >= 0 ){
               this->WindowWidth = InputInt;
            }
         }
         else if( InputString == "height" ){
            if( InputInt >= 0 ){
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

}

void Game::Loop(){
   SDL_Log( "" );
   SDL_Log( "Game: BEGIN\n" );
   while( this->Exit ){
      while( SDL_PollEvent( &this->Event ) ){
         switch( this->Event.type ){
            case SDL_QUIT:
               this->Exit = false;
               SDL_Log( "Window closed\n" );
               break;
            case SDL_MOUSEMOTION:
               this->Mouse.x = -this->Event.motion.xrel;
               this->Mouse.y = -this->Event.motion.yrel;
               this->camera.MouseUpdate( this->Mouse );
               SDL_WarpMouseInWindow( this->Window, this->WindowWidthHalf, this->WindowHeightHalf );
               break;
            case SDL_KEYDOWN:
               switch( this->Event.key.keysym.sym ){
                  case SDLK_ESCAPE:
                     this->Exit = false;
                     SDL_Log( "Window closed\n" );
                     break;
                  case SDLK_w:
                     this->camera.MoveForward();
                     break;
                  case SDLK_s:
                     this->camera.MoveBackward();
                     break;
                  case SDLK_a:
                     this->camera.MoveLeft();
                     break;
                  case SDLK_d:
                     this->camera.MoveRight();
                     break;
                  //Numpad camera:
                  case SDLK_KP_8:
                     this->Mouse.x = 0;
                     this->Mouse.y = 10;
                     this->camera.MouseUpdate( this->Mouse );
                     break;
                  case SDLK_KP_2:
                     this->Mouse.x = 0;
                     this->Mouse.y = -10;
                     this->camera.MouseUpdate( this->Mouse );
                     break;
                  case SDLK_KP_4:
                     this->Mouse.x = 10;
                     this->Mouse.y = 0;
                     this->camera.MouseUpdate( this->Mouse );
                     break;
                  case SDLK_KP_6:
                     this->Mouse.x = -10;
                     this->Mouse.y = 0;
                     this->camera.MouseUpdate( this->Mouse );
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
                     this->WindowWidth = this->Event.window.data1;
                     this->WindowHeight = this->Event.window.data2;
                     this->WindowWidthHalf = this->WindowWidth / 2;
                     this->WindowHeightHalf = this->WindowHeight / 2;
                     this->Aspect = float( this->WindowWidthHalf ) / float( this->WindowHeightHalf );
                     glViewport( 0, 0, (GLsizei)this->WindowWidth, (GLsizei)this->WindowHeight );
                     camera.SetAspect( this->Aspect );
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
      SDL_version compiled, linked;
      SDL_VERSION( &compiled );
      SDL_Log( "Compiled with SDL version: %d.%d.%d\n",
         compiled.major,
         compiled.minor,
         compiled.patch
      );
      SDL_GetVersion( &linked );
      SDL_Log( "Linked with SDL version:   %d.%d.%d\n",
         linked.major,
         linked.minor,
         linked.patch
      );
      if( compiled.major < 2 or linked.major < 2  ){
         return;
      }
      //Check max resolution:
      SDL_Log( "Video and Mode Display info:\n" );
      SDL_Log( "SDL_GetCurrentVideoDriver: %s\n", SDL_GetCurrentVideoDriver() );
      SDL_Log( "SDL_GetNumVideoDisplays %i\n", SDL_GetNumVideoDisplays() );
      for( this->i = 0; this->i < SDL_GetNumVideoDisplays() ; ++this->i ){
         if( SDL_GetCurrentDisplayMode( this->i, &this->CurrentDisplayMode ) != 0 ){
            SDL_Log( "Could not get display mode for video display #%d: %s\n", this->i, SDL_GetError() );
         }
         else{
            SDL_Log( " VideoDisplays #%i:\n", this->i );
            if( SDL_GetDisplayName( this->i ) == NULL ){
               SDL_Log( "SDL_GetDisplayName: %s\n", SDL_GetError() );
            }
            else{
               SDL_Log( "\tSDL_GetDisplayName: %s\n", SDL_GetDisplayName( this->i ) );
            }
            SDL_Log( "\tSDL_GetNumDisplayModes: %i\n", SDL_GetNumDisplayModes( this->i ) );
            for( this->j = 0; this->j < SDL_GetNumDisplayModes( this->i ); ++this->j ){
               if( SDL_GetDisplayMode( this->i, this->j, &this->CurrentDisplayMode ) != 0 ){
                  SDL_Log( "\t\t\tSDL_GetDisplayMode: %s\n", SDL_GetError() );
               }
               else{
                  SDL_Log( "\t   DisplayMode #%i:\t%s = %ibpp\t%iHz\t%ix%i\n", j,
                     SDL_GetPixelFormatName( this->CurrentDisplayMode.format ),
                     SDL_BITSPERPIXEL( this->CurrentDisplayMode.format ),
                     this->CurrentDisplayMode.refresh_rate,
                     this->CurrentDisplayMode.w,
                     this->CurrentDisplayMode.h
                  );
               }
            }
         }
      }
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
      SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
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
      {
         SDL_Log( "" );
         SDL_Log( "Window info:\n" );
         string subsystem = "unknown system";
         SDL_SysWMinfo info;
         SDL_VERSION( &info.version );
         if( SDL_GetWindowWMInfo( this->Window, &info ) ){
            switch( info.subsystem ){
               case SDL_SYSWM_UNKNOWN:
                  break;
               case SDL_SYSWM_WINDOWS:
                  subsystem = "Windows";
                  break;
               case SDL_SYSWM_X11:
                  subsystem = "X Window System";
                  break;
            #if SDL_VERSION_ATLEAST(2, 0, 3)
                  case SDL_SYSWM_WINRT:
                     subsystem = "WinRT";
                     break;
            #endif
               case SDL_SYSWM_DIRECTFB:
                  subsystem = "DirectFB";
                  break;
               case SDL_SYSWM_COCOA:
                  subsystem = "Apple OS X";
                  break;
               case SDL_SYSWM_UIKIT:
                  subsystem = "UIKit";
                  break;
            #if SDL_VERSION_ATLEAST(2, 0, 3)
               case SDL_SYSWM_WAYLAND:
                  subsystem = "Wayland";
                  break;
               case SDL_SYSWM_MIR:
                  subsystem = "Mir";
                  break;
            #endif
            #if SDL_VERSION_ATLEAST(2, 0, 3)
               case SDL_SYSWM_ANDROID:
                  subsystem = "Android";
                  break;
            #endif
               default:
                  break;
            }
            SDL_Log( "SDL_GetWindowWMInfo: %s\n", subsystem.c_str() );
         }
         else{
            SDL_Log( "SDL_GetWindowWMInfo: %s\n", SDL_GetError() );
         }
      }
         this->WindowDisplayIndex = SDL_GetWindowDisplayIndex( this->Window );
         SDL_Log( "SDL_GetWindowDisplayIndex: %i\n", this->WindowDisplayIndex );
         SDL_Log( "SDL_GetWindowID: %i\n", SDL_GetWindowID( this->Window ) );
         SDL_Log( "SDL_GetWindowBrightness: %f\n", SDL_GetWindowBrightness( this->Window ) );
         //Min resolution
         if( SDL_GetDisplayMode(
            this->WindowDisplayIndex,
            SDL_GetNumDisplayModes( this->WindowDisplayIndex ) - 1,
            &this->CurrentDisplayMode
         ) == 0 ){
            this->WindowMinWidth = this->CurrentDisplayMode.w;
            this->WindowMinHeight = this->CurrentDisplayMode.h;
         }
         else{
            SDL_Log( "SDL_GetDisplayMode: %s\n", SDL_GetError() );
         }
         //Max resolution
         if( SDL_GetDisplayMode(
            this->WindowDisplayIndex,
            0,
            &this->CurrentDisplayMode
         ) == 0 ){
            this->WindowMaxWidth = this->CurrentDisplayMode.w;
            this->WindowMaxHeight = this->CurrentDisplayMode.h;
         }
         else{
            SDL_Log( "SDL_GetDisplayMode: %s\n", SDL_GetError() );
         }
         SDL_SetWindowMinimumSize( this->Window, WindowMinWidth, WindowMinHeight );
         SDL_Log( "WindowMinimumSize: %ix%i", WindowMinWidth, WindowMinHeight );
         SDL_SetWindowMaximumSize( this->Window, WindowMaxWidth, WindowMaxHeight );
         SDL_Log( "WindowMaximumSize: %ix%i", WindowMaxWidth, WindowMaxHeight );
         SDL_SetRelativeMouseMode( SDL_TRUE );
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
         this->CheckInit = false;
         return;
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
      this->GL_Error = glewInit();
      if( this->GL_Error != GLEW_OK ){
         SDL_Log( "glewInit: %s\n", glewGetErrorString( this->GL_Error ) );
         this->CheckInit = false;
         return;
      }
      else{
         SDL_Log( "glewInit: SUCCESS\n");
      }
      glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
      glClear( GL_COLOR_BUFFER_BIT );
      glEnable( GL_DEPTH_TEST );
      glDepthFunc( GL_LESS );
      SDL_GL_SwapWindow( this->Window );
      SDL_Log( "OpenGL version: %s", glGetString( GL_VERSION ) );
   }
}

void Game::InitDevIL(){
   if( this->CheckInit ){
      if( ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION ) ||
          ( ilGetInteger( ILU_VERSION_NUM ) < ILU_VERSION )
      ){
         SDL_Log( "DevIL: version problem!\n" );
         this->CheckInit = false;
         return;
      }
      else{
         ilInit();
         iluInit();
         this->IL_Error = IL_NO_ERROR;
         while( ( this->IL_Error = ilGetError() ) != IL_NO_ERROR ){
            SDL_Log( "DevIL: %s\n", iluErrorString( this->IL_Error ) );
         }
         if( this->IL_Error == IL_NO_ERROR ){
            SDL_Log( "ilInit : SUCCESS\n" );
            SDL_Log( "iluInit: SUCCESS\n" );
         }
      }
   }
}
