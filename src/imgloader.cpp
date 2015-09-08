#include "imgloader.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <IL/il.h>
#include <IL/ilu.h>
/* for include ilut.h:
#ifndef ILUT_USE_OPENGL
   #define ILUT_USE_OPENGL
#endif
#include <IL/ilut.h>
*/

bool LoadImg( const char *img_path_file, GLuint &image ){
   SDL_Log( "Loading image: %s", img_path_file );
   ILenum error;
   GLenum error_gl;
   ILboolean success;
   ILuint imgage_id;

   ilGenImages( 1, &imgage_id );
   ilBindImage( imgage_id );
   success = ilLoadImage( img_path_file );
   if( ! success ){
      error = ilGetError();
      if( error != IL_NO_ERROR ){
         SDL_LogError( SDL_LOG_CATEGORY_INPUT, "ilLoadImage: %s\n", iluErrorString( error ) );
         image = 0;
         ilDeleteImages( 1, &imgage_id );
         return false;
      }
      SDL_LogError( SDL_LOG_CATEGORY_INPUT, "ilLoadImage: %d\n", ilGetError() );
      image = 0;
      ilDeleteImages( 1, &imgage_id );
      return false;
   }

   GLint width = ilGetInteger( IL_IMAGE_WIDTH );
   GLint height = ilGetInteger( IL_IMAGE_HEIGHT );
   GLint type = ilGetInteger( IL_IMAGE_TYPE );
   GLint format = ilGetInteger( IL_IMAGE_FORMAT );

   glGenTextures( 1, &image );
   glBindTexture( GL_TEXTURE_2D, image );

   glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, type, ilGetData() );
   error_gl = glGetError();
   if( error_gl != GL_NO_ERROR and error_gl != GL_INVALID_ENUM ){
      SDL_LogError( SDL_LOG_CATEGORY_ERROR, "glTexImage2D:, %s\n", gluErrorString( error_gl ) );
      return false;
   }

   glGenerateMipmap( GL_TEXTURE_2D );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glBindTexture(GL_TEXTURE_2D, 0);

   ilDeleteImages( 1, &imgage_id );
   SDL_Log( "Loaded image: %s", img_path_file );
   return true;
}

GLuint LoadImg( const char *img_path_file ){
   SDL_Log( "Loading image: %s", img_path_file );
   ILenum error;
   GLenum error_gl;
   ILboolean success;
   ILuint imgage_id;

   ilGenImages( 1, &imgage_id );
   ilBindImage( imgage_id );
   success = ilLoadImage( img_path_file );
   if( ! success ){
      error = ilGetError();
      if( error != IL_NO_ERROR ){
         SDL_LogError( SDL_LOG_CATEGORY_INPUT, "ilLoadImage: %s\n", iluErrorString( error ) );
         ilDeleteImages( 1, &imgage_id );
         return 0;
      }
      SDL_LogError( SDL_LOG_CATEGORY_INPUT, "ilLoadImage: %d\n", ilGetError() );
      ilDeleteImages( 1, &imgage_id );
      return 0;
   }

   GLint width = ilGetInteger( IL_IMAGE_WIDTH );
   GLint height = ilGetInteger( IL_IMAGE_HEIGHT );
   GLint type = ilGetInteger( IL_IMAGE_TYPE );
   GLint format = ilGetInteger( IL_IMAGE_FORMAT );

   GLuint image;
   glGenTextures( 1, &image );
   glBindTexture( GL_TEXTURE_2D, image );

   glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, type, ilGetData() );
   error_gl = glGetError();
   if( error_gl != GL_NO_ERROR and error_gl != GL_INVALID_ENUM ){
      SDL_LogError( SDL_LOG_CATEGORY_ERROR, "glTexImage2D:, %s\n", gluErrorString( error_gl ) );
      return 0;
   }

   glGenerateMipmap( GL_TEXTURE_2D );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glBindTexture(GL_TEXTURE_2D, 0);

   ilDeleteImages( 1, &imgage_id );
   SDL_Log( "Loaded image: %s", img_path_file );
   return image;
}
