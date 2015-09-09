/*!
   \file shader.cpp
   \brief Plik źródłowy dla shader.hpp.
*/
#include "shader.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

GLuint LoadShader( const char* vertex_shader_path_file,
   const char* fragment_shader_path_file
){
   SDL_Log( "Creating shaders\n" );
   GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
   GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
   SDL_Log( "Created shaders\n" );

   std::string Line;

   //Load code vertex_shader:
   SDL_Log( "Loading: %s\n", vertex_shader_path_file );
   std::string VertexShaderCode;
   std::ifstream VertexShaderStream( vertex_shader_path_file );
   if( VertexShaderStream.good() ){
      Line = "";
      while( getline( VertexShaderStream, Line ) ){
         VertexShaderCode += Line + "\n";
      }
      VertexShaderStream.close();
   }
   else{
      SDL_LogError( SDL_LOG_CATEGORY_INPUT, "Can't find file: %s\n", vertex_shader_path_file );
      return 0;
   }
   SDL_Log( "Loaded: %s\n", vertex_shader_path_file );

   //Load code fragment_shader:
   SDL_Log( "Loading: %s\n", fragment_shader_path_file );
   std::string FragmentShaderCode;
   std::ifstream FragmentShaderStream( fragment_shader_path_file );
   if( FragmentShaderStream.good() ){
      Line = "";
      while( getline( FragmentShaderStream, Line ) ){
         FragmentShaderCode += Line + "\n";
      }
      FragmentShaderStream.close();
   }
   else{
      SDL_LogError( SDL_LOG_CATEGORY_INPUT, "Can't find file: %s\n", fragment_shader_path_file );
      return 0;
   }
   SDL_Log( "Loaded: %s\n", fragment_shader_path_file );

   GLint Result;
   GLint InfoLogLength;
   std::string Error;

   //Compile vertex_shader:
   SDL_Log( "Compiling vertex shader: %s\n", vertex_shader_path_file );
   const char *VertexShaderCodePointer = VertexShaderCode.c_str();
   glShaderSource( VertexShaderID, 1, &VertexShaderCodePointer, 0 );
   glCompileShader( VertexShaderID );
   //Check vertex_shader:
   Result = GL_FALSE;
   glGetShaderiv( VertexShaderID, GL_COMPILE_STATUS, &Result );
   if( Result == GL_FALSE ){
      InfoLogLength = 0;
      glGetShaderiv( VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
      std::vector <GLchar> VertexShaderErrorMessage(InfoLogLength);
      glGetShaderInfoLog( VertexShaderID, InfoLogLength, &InfoLogLength, &VertexShaderErrorMessage[0] );
      Error = std::string( VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end() );
      SDL_LogError( SDL_LOG_CATEGORY_ERROR, " Error: %s\n", Error.c_str() );
      glDeleteShader( VertexShaderID );
      return 0;
   }
   SDL_Log( "Compiled vertex shader: %s\n", vertex_shader_path_file );

   //Compile fragment_shader:
   SDL_Log( "Compiling fragment shader: %s\n", fragment_shader_path_file );
   const char *FragmentShaderCodePointer = FragmentShaderCode.c_str();
   glShaderSource( FragmentShaderID, 1, &FragmentShaderCodePointer, 0 );
   glCompileShader( FragmentShaderID );
   //Check fragment_shader:
   Result = GL_FALSE;
   glGetShaderiv( FragmentShaderID, GL_COMPILE_STATUS, &Result );
   if( Result == GL_FALSE ){
      InfoLogLength = 0;
      glGetShaderiv( FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength );
      std::vector <GLchar> FragmentShaderErrorMessage(InfoLogLength);
      glGetShaderInfoLog( FragmentShaderID, InfoLogLength, &InfoLogLength, &FragmentShaderErrorMessage[0] );
      Error.clear();
      Error = std::string( FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end() );
      SDL_LogError( SDL_LOG_CATEGORY_ERROR, " Error: %s\n", Error.c_str() );
      glDeleteShader( VertexShaderID );
      glDeleteShader( FragmentShaderID );
      return 0;
   }
   SDL_Log( "Compiled fragment shader: %s\n", fragment_shader_path_file );

   Error.clear();
   Result = GL_FALSE;
   InfoLogLength = 0;

   //Link program:
   SDL_Log( "Linking program\n" );
   GLuint ProgramID = glCreateProgram();
   glAttachShader( ProgramID, VertexShaderID );
   glAttachShader( ProgramID, FragmentShaderID );
   glLinkProgram( ProgramID );

   //Check program:
   glGetProgramiv( ProgramID, GL_LINK_STATUS, &Result );
   if( Result == GL_FALSE ){
      glGetProgramiv( ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength );
      std::vector <GLchar> ProgramErrorMessage(InfoLogLength);
      glGetProgramInfoLog( ProgramID, InfoLogLength, &InfoLogLength, &ProgramErrorMessage[0] );
      Error.clear();
      Error = std::string( ProgramErrorMessage.begin(), ProgramErrorMessage.end() );
      SDL_LogError( SDL_LOG_CATEGORY_ERROR, " Error: %s\n", Error.c_str() );
      glDeleteProgram( ProgramID );
      glDeleteShader( VertexShaderID );
      glDeleteShader( FragmentShaderID );
      return 0;
   }
   SDL_Log( "Linked program\n" );

   //delete shaders:
   glDetachShader( ProgramID, VertexShaderID );
   glDetachShader( ProgramID, FragmentShaderID );
   glDeleteShader( VertexShaderID );
   glDeleteShader( FragmentShaderID );

   return ProgramID;
}
