#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
//OpenGL:
#define GLEW_STATIC
#include <GL/glew.h>
//SDL:
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h> //SDL_SysWMinfo
#include <SDL2/SDL_opengl.h>
//glm:
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/setup.hpp>
#if GLM_VERSION >= 96
#include <glm/gtc/vec1.hpp>
#else
#include <glm/gtx/vec1.hpp>
#endif
//DevIL:
#include <IL/il.h>
#include <IL/ilu.h>
//CaughtSignal:
#include <signal.h>
//User include
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "light.hpp"

using namespace std;

void CaughtSignal( int signal );

#define Max_Point_Light 1
#if Max_Point_Light < 1
   #undef Max_Point_Light
   #define Max_Point_Light 1
#endif

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
   void InitShaders();
   void LoadData();
   inline void RandNewCoin();
   inline void CheckCoin();
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
   bool Focus = true;
//Timer:
   Uint32 TimerBegin = 0;
   Uint32 TimerEnd = 0;
   Uint32 TimerUpdate = 0;
   Uint32 TimerUpdateTime = 50;
   int FPS = 0;
   //Game:
   inline void Loop();
   inline void Update();
   bool Exit = true;
   SDL_Event Event;
   const int MapMax = 30;
   const int MapMaxHalf = MapMax / 2;
   vector < vector <int> > Map;
   vector < vector <int> > MapIndex;
   int Score = 0;
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
//Shaders:
   GLuint ProgramID = 0;
   //Uniforms:
   GLuint ModelUniformId = 0;
   GLuint ViewUniformId = 0;
   GLuint ProjectionUniformId = 0;
   GLuint TextureUniformId = 0;
   GLuint TextureSpecularUniformId = 0;
   GLuint AmbientUniformId = 0;
   GLuint DiffuseUniformId = 0;
   GLuint SpecularUniformId = 0;
   GLuint ShininessUniformId = 0;
   //Directional Light:
   GLuint LightPositionUniformId = 0;
   GLuint LightAmbientUniformId = 0;
   GLuint LightDiffuseUniformId = 0;
   GLuint LightSpecularUniformId = 0;
   //Camera Position:
   GLuint ViewPosUniformId = 0;
   //Point Light:
   GLuint PointLight_Position_Uniform[Max_Point_Light];
   GLuint PointLight_Constant_Uniform[Max_Point_Light];
   GLuint PointLight_Linear_Uniform[Max_Point_Light];
   GLuint PointLight_Quadratic_Uniform[Max_Point_Light];

//second for drawing light object:
   GLuint LightID = 0;
   //Uniforms:
   GLuint ViewUniformLight = 0;
   GLuint ProjectionUniformLight = 0;
   GLuint ModelUniformLight = 0;
   GLuint UniformColorLight = 0;
//Matrix:
   mat4 ProjectionMatrix;
   mat4 ViewMatrix;
//Models:
   vector <Model> Models;
   vector <Model>::iterator It;
//Light:
   Light Sun;
   Light SunMoving;
   vec3 SunMovingPosition = vec3( 0.0f, 15.0f, 0.0f );
   GLfloat SunMovingDegreese = 0.0f;
   GLfloat SunMovingRadian = 0.0f;
   GLfloat SunMovingRadius = 15.0f;
//tmp:
   vec3 tmp_vector;
   GLfloat tmp_float;
   vec3 RotateVec = vec3( 0.0f, 1.0f, 0.0f );
   GLfloat RotateFloat = 5;
   int tmp_x, tmp_y, tmp_z;
};

Game * PointerGame = NULL;
void GlobalStop();

int main( int argc, char* argv[] ){
   #ifdef SIGTSTP
   signal( SIGTSTP, CaughtSignal );
   #endif
   signal( SIGINT, CaughtSignal );
   Game game;
   PointerGame = &game;
   game.Start();
   PointerGame = NULL;
   return 0;
}

void CaughtSignal( int signal ){
   #ifdef SIGTSTP
   //Linux:
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
   #else
   //Windows:
   if( signal == SIGINT ){
      cout<<"\n\nCaught signal SIGINT\n\n";
      GlobalStop();
   }
   #endif
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
   srand( time( 0 ) );
   this->Map.resize( this->MapMax, vector <int> ( this->MapMax, -1 ) );
   this->MapIndex.resize( this->MapMax, vector <int> ( this->MapMax, -1 ) );
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
   this->Map.clear();
   this->MapIndex.clear();
   SDL_Log( "Destructor: CLEANING\n" );
   Model::ModelUniformId = NULL;
   Model::TextureUniformId = NULL;
   Model::TextureSpecularUniformId = NULL;
   Model::AmbientUniformId = NULL;
   Model::DiffuseUniformId = NULL;
   Model::SpecularUniformId = NULL;
   Model::ShininessUniformId = NULL;
   Model::ModelUniformLight = NULL;
   Model::UniformColorLight = NULL;
   Light::ModelUniformLight = NULL;
   Light::UniformColorLight = NULL;
   glDeleteProgram( this->ProgramID );
   glDeleteProgram( this->LightID );
   SDL_SetRelativeMouseMode( SDL_FALSE );
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
   this->InitShaders();

   this->LoadData();

   this->TimerBegin = SDL_GetTicks();
   this->TimerEnd = this->TimerBegin + 1000;
   this->TimerUpdate = this->TimerBegin;

   this->Exit = this->CheckInit;

   this->Loop();

   SDL_Log( "\rYOUR SCORE: %i\n", this->Score );
}

void Game::Loop(){
   SDL_Log( "\n" );
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
                  case SDLK_e:
                     this->CheckCoin();
                     break;
                  case SDLK_SPACE:
                     this->camera.MoveUp();
                     break;
                  case SDLK_LCTRL:
                  case SDLK_c:
                     this->camera.MoveDown();
                     break;
                  case SDLK_BACKQUOTE:
                     this->camera.Log();
                     this->Sun.Log();
                     this->SunMoving.Log();
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
                  case SDLK_F7:
                     this->camera.TurnFreeCamera();
                     break;
                  case SDLK_F10:
                     this->camera.SetPositionDefault();
                     break;
                  case SDLK_F12:
                     if( this->FullScreen ){
                        this->FullScreen = false;
                        SDL_SetWindowFullscreen( this->Window, 0 );
                     }
                     else{
                        this->FullScreen = true;
                        SDL_SetWindowFullscreen( this->Window, SDL_WINDOW_FULLSCREEN );
                        //SDL_SetWindowFullscreen( this->Window, SDL_WINDOW_FULLSCREEN_DESKTOP );
                     }
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
                     this->Aspect = vec1( float( this->WindowWidthHalf ) / float( this->WindowHeightHalf ) );
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
                  case SDL_WINDOWEVENT_FOCUS_GAINED:
                     SDL_Log( "Window %d gained keyboard focus\n", this->Event.window.windowID );
                     SDL_SetRelativeMouseMode( SDL_TRUE );
                     this->Focus = true;
                     break;
                  case SDL_WINDOWEVENT_FOCUS_LOST:
                     SDL_Log( "Window %d lost keyboard focus\n", this->Event.window.windowID );
                     SDL_SetRelativeMouseMode( SDL_FALSE );
                     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                     SDL_GL_SwapWindow( this->Window );
                     this->Focus = false;
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
   if( this->Focus ){
      this->TimerBegin = SDL_GetTicks();
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      this->ProjectionMatrix = this->camera.getProjectionMatrix();
      this->ViewMatrix = this->camera.getViewMatrix();

      //Draw all models:
      glUseProgram( this->ProgramID );

      //Matrix:
      glUniformMatrix4fv( this->ViewUniformId, 1, GL_FALSE, value_ptr( this->ViewMatrix ) );
      glUniformMatrix4fv( this->ProjectionUniformId, 1, GL_FALSE, value_ptr( this->ProjectionMatrix  ) );

      //Directional light:
      glUniform3fv( this->LightPositionUniformId, 1, value_ptr( this->Sun.ReturnPosition() ) );
      glUniform3fv( this->LightAmbientUniformId , 1, value_ptr( this->Sun.ReturnAmbient() ) );
      glUniform3fv( this->LightDiffuseUniformId , 1, value_ptr( this->Sun.ReturnDiffuse() ) );
      glUniform3fv( this->LightSpecularUniformId , 1, value_ptr( this->Sun.ReturnSpecular() ) );

      //Camera position:
      glUniform3fv( this->ViewPosUniformId, 1, value_ptr( this->camera.ReturnPosition() ) );

      //Point light:
      glUniform3fv( PointLight_Position_Uniform[0], 1, value_ptr( this->SunMoving.ReturnPosition() ) );
      /*
      Distance     Constant     Linear     Quadratic
      7            1.0          0.7        1.8
      13           1.0          0.35       0.44
      20           1.0          0.22       0.20
      32           1.0          0.14       0.07
      50           1.0          0.09       0.032
      65           1.0          0.07       0.017
      100          1.0          0.045      0.0075
      160          1.0          0.027      0.0028
      200          1.0          0.022      0.0019
      325          1.0          0.014      0.0007
      600          1.0          0.007      0.0002
      3250         1.0          0.0014     0.000007
      */
      glUniform1f( PointLight_Constant_Uniform[0], 1.0f );
      glUniform1f( PointLight_Linear_Uniform[0], 0.07f );
      glUniform1f( PointLight_Quadratic_Uniform[0], 0.017f );

      //Draw all objects:
      for( this->It = this->Models.begin();this->It != this->Models.end(); ++this->It ){
         this->It->Draw();
      }

      //Draw lights:
      glUseProgram( this->LightID );
      glUniformMatrix4fv( this->ViewUniformLight, 1, GL_FALSE, value_ptr( this->ViewMatrix ) );
      glUniformMatrix4fv( this->ProjectionUniformLight, 1, GL_FALSE, value_ptr( this->ProjectionMatrix  ) );

      //Draw Collision Square:
      /*
      for( this->It = this->Models.begin();this->It != this->Models.end(); ++this->It ){
         this->It->DrawCollisionSquare();
      }
      */

      //Draw light 1:
      this->Sun.Draw();
      //Draw light 2:
      this->SunMoving.Draw();

      glUseProgram( 0 );
      SDL_GL_SwapWindow( this->Window );

      if( this->TimerBegin >= this->TimerUpdate ){
         //Rotate coin:
         this->Models[1].Rotate( this->RotateFloat, this->RotateVec );
         this->TimerUpdate = this->TimerBegin + this->TimerUpdateTime;

         //Move second light:
         this->SunMovingDegreese = fmod( this->SunMovingDegreese, 360.0f );
         this->SunMovingRadian = this->SunMovingDegreese * ( M_PI / 180 );
         this->SunMovingPosition.x =  this->SunMovingRadius * sin( this->SunMovingRadian );
         this->SunMovingPosition.z = this->SunMovingRadius * cos( this->SunMovingRadian );
         this->SunMoving.ChangePosition( this->SunMovingPosition );
         this->SunMovingDegreese += 0.25f;
      }
      ++this->FPS;
      if( this->TimerBegin >= this->TimerEnd ){
         SDL_Log( "\r[%i] FPS: %i", this->TimerBegin / 1000, this->FPS );
         this->FPS = 0;
         this->TimerEnd  = this->TimerBegin + 1000;
      }
   }
}

void Game::InitSDL(){
   if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
      SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_Init: %s\n", SDL_GetError() );
      this->CheckInit = false;
   }
   else{
      SDL_LogSetAllPriority( SDL_LOG_PRIORITY_INFO );
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
            SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "Could not get display mode for video display #%d: %s\n", this->i, SDL_GetError() );
         }
         else{
            SDL_Log( " VideoDisplays #%i:\n", this->i );
            if( SDL_GetDisplayName( this->i ) == NULL ){
               SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_GetDisplayName: %s\n", SDL_GetError() );
            }
            else{
               SDL_Log( "\tSDL_GetDisplayName: %s\n", SDL_GetDisplayName( this->i ) );
            }
            SDL_Log( "\tSDL_GetNumDisplayModes: %i\n", SDL_GetNumDisplayModes( this->i ) );
            for( this->j = 0; this->j < SDL_GetNumDisplayModes( this->i ); ++this->j ){
               if( SDL_GetDisplayMode( this->i, this->j, &this->CurrentDisplayMode ) != 0 ){
                  SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "\t\t\tSDL_GetDisplayMode: %s\n", SDL_GetError() );
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
      SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 2 );
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
         SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_CreateWindow: %s\n", SDL_GetError() );
         this->CheckInit = false;
      }
      else{
      {
         SDL_Log( "\n" );
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
            #ifdef SDL_SYSWM_ANDROID
               case SDL_SYSWM_ANDROID:
                  subsystem = "Android";
                  break;
            #endif
            #endif
               default:
                  break;
            }
            SDL_Log( "SDL_GetWindowWMInfo: %s\n", subsystem.c_str() );
         }
         else{
            SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_GetWindowWMInfo: %s\n", SDL_GetError() );
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
            SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_GetDisplayMode: %s\n", SDL_GetError() );
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
            SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_GetDisplayMode: %s\n", SDL_GetError() );
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
         SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_SetWindowIcon: %s\n", SDL_GetError() );
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
         SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "SDL_GL_CreateContext: %s\n", SDL_GetError() );
         this->CheckInit = false;
         return;
      }
      SDL_Log( "SDL_GL_CreateContext: SUCCESS\n" );
      this->GL_Error = glewInit();
      if( this->GL_Error != GLEW_OK ){
         SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "glewInit: %s\n", glewGetErrorString( this->GL_Error ) );
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
      // glEnable( GL_CULL_FACE );
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      SDL_GL_SwapWindow( this->Window );
      SDL_Log( "OpenGL version: %s", glGetString( GL_VERSION ) );
   }
}

void Game::InitDevIL(){
   if( this->CheckInit ){
      if( ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION ) ||
          ( ilGetInteger( ILU_VERSION_NUM ) < ILU_VERSION )
      ){
         SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "DevIL: version problem!\n" );
         this->CheckInit = false;
         return;
      }
      else{
         ilInit();
         iluInit();
         this->IL_Error = IL_NO_ERROR;
         while( ( this->IL_Error = ilGetError() ) != IL_NO_ERROR ){
            SDL_LogCritical( SDL_LOG_CATEGORY_SYSTEM, "DevIL: %s\n", iluErrorString( this->IL_Error ) );
         }
         if( this->IL_Error == IL_NO_ERROR ){
            SDL_Log( "ilInit : SUCCESS\n" );
            SDL_Log( "iluInit: SUCCESS\n" );
         }
      }
   }
}

void Game::InitShaders(){
   if( this->CheckInit ){
      SDL_Log( "\n" );
      SDL_Log( "SHADERS:\n" );
      this->ProgramID = LoadShader( "./data/Shader.vert", "./data/Shader.frag" );
      if( this->ProgramID == 0 ){
         SDL_LogCritical( SDL_LOG_CATEGORY_INPUT, "Something wrong with program shaders!\n" );
         this->CheckInit = false;
         return;
      }
      SDL_Log( "\n" );
      this->LightID = LoadShader( "./data/Light.vert", "./data/Light.frag" );
      if( this->LightID == 0 ){
         SDL_LogCritical( SDL_LOG_CATEGORY_INPUT, "Something wrong with light shaders!\n" );
         this->CheckInit = false;
         return;
      }

      //Uniforms:
      this->ViewUniformId = glGetUniformLocation( this->ProgramID, "view" );
      this->ProjectionUniformId = glGetUniformLocation( this->ProgramID, "projection" );
      this->ModelUniformId = glGetUniformLocation( this->ProgramID, "model" );

      //Single object:
      this->TextureUniformId  = glGetUniformLocation( this->ProgramID, "Material.Texture" );
      this->TextureSpecularUniformId = glGetUniformLocation( this->ProgramID, "Material.Texture_specular" );
      this->AmbientUniformId = glGetUniformLocation( this->ProgramID, "Material.Ambient" );
      this->DiffuseUniformId = glGetUniformLocation( this->ProgramID, "Material.Diffuse" );
      this->SpecularUniformId = glGetUniformLocation( this->ProgramID, "Material.Specular" );
      this->ShininessUniformId = glGetUniformLocation( this->ProgramID, "Material.Shininess" );

      //Directional Light:
      this->LightPositionUniformId = glGetUniformLocation( this->ProgramID, "DirectionalLight.Position" );
      this->LightAmbientUniformId = glGetUniformLocation( this->ProgramID, "DirectionalLight.Ambient" );
      this->LightDiffuseUniformId = glGetUniformLocation( this->ProgramID, "DirectionalLight.Diffuse" );
      this->LightSpecularUniformId = glGetUniformLocation( this->ProgramID, "DirectionalLight.Specular" );

      //Camera position:
      this->ViewPosUniformId = glGetUniformLocation( this->ProgramID, "ViewPos" );

      //Point Light
      string i_to_string, uniform_string;
      for( int i = 0; i < Max_Point_Light; ++i ){
         i_to_string = to_string( i );
         uniform_string = "PointLight[" + i_to_string + "].Position";
         PointLight_Position_Uniform[i] = glGetUniformLocation( this->ProgramID, uniform_string.c_str() );
         uniform_string = "PointLight[" + i_to_string + "].Constant";
         PointLight_Constant_Uniform[i] = glGetUniformLocation( this->ProgramID, uniform_string.c_str() );
         uniform_string = "PointLight[" + i_to_string + "].Linear";
         PointLight_Linear_Uniform[i] = glGetUniformLocation( this->ProgramID, uniform_string.c_str() );
         uniform_string = "PointLight[" + i_to_string + "].Quadratic";
         PointLight_Quadratic_Uniform[i] = glGetUniformLocation( this->ProgramID, uniform_string.c_str() );
      }

      this->ViewUniformLight = glGetUniformLocation( this->LightID, "view" );
      this->ProjectionUniformLight = glGetUniformLocation( this->LightID, "projection" );
      this->ModelUniformLight = glGetUniformLocation( this->LightID, "model" );
      this->UniformColorLight = glGetUniformLocation( this->LightID, "Color" );


      //Set pointer for Model:
      Model::ModelUniformId = & this->ModelUniformId;
      Model::TextureUniformId = & this->TextureUniformId;
      Model::TextureSpecularUniformId = & this->TextureSpecularUniformId;
      Model::AmbientUniformId = & this->AmbientUniformId;
      Model::DiffuseUniformId = & this->DiffuseUniformId;
      Model::SpecularUniformId = & this->SpecularUniformId;
      Model::ShininessUniformId = & this->ShininessUniformId;
      Model::ModelUniformLight = &this->ModelUniformLight;
      Model::UniformColorLight = & this->UniformColorLight;

      Light::ModelUniformLight = & this->ModelUniformLight;
      Light::UniformColorLight = & this->UniformColorLight;

   }
}

void Game::LoadData(){
   SDL_Log( "\n" );
   if( this->CheckInit ){
      //Load other models from ./data/data.init:
      fstream DataFile;
      DataFile.open( "./data/data.init", ios::in );
      if( DataFile.good() ){
         SDL_Log( "Open file: ./data/data.init\n" );
         istringstream Input;
         string Line;
         const string Dir = "./data/";
         string Word;
         while( getline( DataFile, Line ) ){
            Input.str( "" );
            Input.clear();
            Input.str( Line );
            Model tmp_model;
            Input>>Word;
            tmp_model.SetName( Word );
            Input>>Word;
            tmp_model.SetOBJPathFile( Dir + Word );
            Input>>Word;
            tmp_model.SetMTLPathFile( Dir + Word );
            Input>>Word;
            tmp_model.SetImgPathFile( Dir + Word );
            Input>>Word;
            tmp_model.SetImgSpecPathFile( Dir + Word );
            this->Models.push_back( tmp_model );
         }
         DataFile.close();
      }
      else{
         DataFile.close();
         SDL_LogCritical( SDL_LOG_CATEGORY_INPUT, "Can't find file: ./data/data.init\n" );
         this->CheckInit = false;
         return;
      }

      //Check is empty:
      if( this->Models.empty() ){
         SDL_LogCritical( SDL_LOG_CATEGORY_INPUT, "Can't load data models from: ./data/data.init\n" );
         this->CheckInit = false;
         return;
      }

      //Check exist file:
      DataFile.open( "./data/sun.obj", ios::in );
      if( ! DataFile.good() ){
         DataFile.close();
         SDL_LogCritical( SDL_LOG_CATEGORY_INPUT, "Can't load data models from: ./data/sun.obj\n" );
         this->CheckInit = false;
         return;
      }
      DataFile.close();

      //Create world:
      SDL_Log( "Creating world\n" );
      int X, Y;
      int Counter = 0;
      int CounterMax = this->Models.size() - 2;
      int CounterTypeMax = CounterMax;
      vector <int> CouterType( CounterTypeMax, 0 );
      int tmp;
      vec3 VecRand;
      while( ( CounterMax * 2 ) < ( 0.7 * this->MapMax ) ){
         CounterMax *= 2;
      }
      while( Counter < CounterMax ){
         for( tmp = 0; tmp < CounterTypeMax; ++tmp ){
            do{
               X = rand() % this->MapMax;
               Y = rand() % this->MapMax;
            }while( this->Map[Y][X] != -1 );
            this->Map[Y][X] = tmp + 2;
            this->MapIndex[Y][X] = CouterType[tmp];
            X -= this->MapMaxHalf;
            Y -= this->MapMaxHalf;
            ++CouterType[tmp];
            VecRand = vec3( X, 0, Y );
            this->Models[tmp+2].AddMatrix( VecRand );
            ++Counter;
         }
      }
      SDL_Log( "Created world\n" );

      //Add coin:
      do{
         X = rand() % this->MapMax;
         Y = rand() % this->MapMax;
      }while( this->Map[Y][X] != -1 );
      //Coin number in memory is 1 and index 0 (first coin):
      this->Map[Y][X] = 1;
      this->MapIndex[Y][X] = 0;
      X -= this->MapMaxHalf;
      Y -= this->MapMaxHalf;
      VecRand = vec3( X, 0.25f, Y );
      this->Models[1].AddMatrix( VecRand );
      VecRand = vec3( 0.5f );
      this->Models[1].Scale( VecRand );

      //Load into memory:
      for( this->It = this->Models.begin();this->It != this->Models.end(); ++this->It ){
         this->It->Load();
      }

      //Set and load main light:
      this->Sun.SetPath( "./data/sun.obj" );
      this->tmp_vector = vec3( 0.0f, 15.0f, 0.0f );
      this->Sun.Translate( this->tmp_vector );
      this->Sun.Load();
      this->tmp_vector = vec3( 0.25f );
      this->Sun.SetAmbient( this->tmp_vector );
      this->tmp_vector = vec3( 0.25f );
      this->Sun.SetDiffuse( this->tmp_vector );
      this->tmp_vector = vec3( 0.25f );
      this->Sun.SetSpecular( this->tmp_vector );

      //Second light:
      this->SunMoving.SetPath( "./data/sun.obj" );
      this->tmp_vector = vec3( 0.5f, 0.5f, 0.5f );
      this->SunMoving.Scale( this->tmp_vector );
      //set defualt position:
      this->SunMovingDegreese = 0.0f;
      this->SunMovingRadian = 0.0f;
      this->SunMovingRadius = 15.0f;
      this->SunMovingPosition.x = this->SunMovingRadius * sin( this->SunMovingRadian );
      this->SunMovingPosition.y = 10.0f;
      this->SunMovingPosition.z = this->SunMovingRadius * cos( this->SunMovingRadian );
      this->SunMoving.ChangePosition( this->SunMovingPosition );
      this->SunMovingDegreese += 0.25f;
      this->SunMoving.Load();

      //Set min/max movement:
      this->tmp_vector = vec3( -this->MapMaxHalf, -5.0f, -this->MapMaxHalf );
      this->camera.SetPositionMin( this->tmp_vector );
      this->tmp_vector = vec3( this->MapMaxHalf, 15.0f, this->MapMaxHalf );
      this->camera.SetPositionMax( this->tmp_vector );
   }
}

void Game::RandNewCoin(){
   this->Map[this->tmp_z][this->tmp_x] = -1;
   this->MapIndex[this->tmp_z][this->tmp_x] = -1;
   do{
      this->tmp_x = rand() % this->MapMax;
      this->tmp_z = rand() % this->MapMax;
   }while( this->Map[this->tmp_z][this->tmp_x] != -1 );
   this->Map[this->tmp_z][this->tmp_x] = 1;
   this->MapIndex[this->tmp_z][this->tmp_x] = 0;
   this->tmp_x -= this->MapMaxHalf;
   this->tmp_z -= this->MapMaxHalf;
   this->tmp_vector = vec3( this->tmp_x, 0.0f, this->tmp_z );
   this->Models[1].ChangeMatrix( 0, this->tmp_vector );
   ++this->Score;
}

void Game::CheckCoin(){
   this->tmp_vector = camera.ReturnPosition();
   this->tmp_x = (int)( this->tmp_vector.x + this->MapMaxHalf );
   this->tmp_z = (int)( this->tmp_vector.z + this->MapMaxHalf );
   if( ( this->tmp_x > 0 ) and ( this->tmp_x < this->MapMax - 1 ) ){
      if( ( this->tmp_z > 0 ) and ( this->tmp_z < this->MapMax - 1 ) ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
      }
      else if( this->tmp_z == 0 ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
      }
      else if( this->tmp_z == ( this->MapMax - 1 ) ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
      }
   }
   else if( this->tmp_x == 0 ){
      if( ( this->tmp_z > 0 ) and ( this->tmp_z < this->MapMax - 1 ) ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
      }
      else if( this->tmp_z == 0 ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
      }
      else if( this->tmp_z == ( this->MapMax - 1 ) ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x+1] == 1 ){
            this->RandNewCoin();
         }
      }
   }
   else if( this->tmp_x == ( this->MapMax - 1 ) ){
      if( ( this->tmp_z > 0 ) and ( this->tmp_z < this->MapMax - 1 ) ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
      }
      else if( this->tmp_z == 0 ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z+1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
      }
      else if( this->tmp_z == ( this->MapMax - 1 ) ){
         if( this->Map[this->tmp_z][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x] == 1 ){
            this->RandNewCoin();
         }
         else if( this->Map[this->tmp_z-1][this->tmp_x-1] == 1 ){
            this->RandNewCoin();
         }
      }
   }
}
