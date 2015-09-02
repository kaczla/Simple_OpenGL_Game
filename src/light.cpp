#include "light.hpp"
#include <SDL2/SDL.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objloader.hpp"

GLuint * Light::ModelUniformLight = NULL;
GLuint * Light::UniformColorLight = NULL;

Light::Light(){
   this->VAO = 0;
   this->VertexBuffer = 0;
   this->IndicesBuffer = 0;

   this->Color = glm::vec3( 1.0f, 1.0f, 1.0f );
}

Light::Light( std::string path_obj ){
   this->VAO = 0;
   this->VertexBuffer = 0;
   this->IndicesBuffer = 0;

   this->Color = glm::vec3( 1.0f, 1.0f, 1.0f );

   this->OBJPathFile = path_obj;
}

Light::Light( const Light &light ){
   this->VAO = light.VAO;

   this->Vertices = light.Vertices;
   this->Uvs = light.Uvs;
   this->Normals = light.Normals;
   this->Indices = light.Indices;

   this->VertexBuffer = light.VertexBuffer;
   this->IndicesBuffer = light.IndicesBuffer;

   this->OBJPathFile = light.OBJPathFile;

   this->ModelMatrix = light.ModelMatrix;
   this->Color = light.Color;

   this->Init = light.Init;
}

Light::~Light(){
   glDeleteBuffers( 1, &this->VertexBuffer );
   glDeleteBuffers( 1, &this->IndicesBuffer );
   glDeleteVertexArrays( 1, &this->VAO );
}

Light & Light::operator=( const Light &light ){
   this->VAO = light.VAO;

   this->Vertices = light.Vertices;
   this->Uvs = light.Uvs;
   this->Normals = light.Normals;
   this->Indices = light.Indices;

   this->VertexBuffer = light.VertexBuffer;
   this->IndicesBuffer = light.IndicesBuffer;

   this->OBJPathFile = light.OBJPathFile;

   this->ModelMatrix = light.ModelMatrix;
   this->Color = light.Color;

   this->Init = light.Init;
}

void Light::SetOBJPathFile( std::string path ){
   this->OBJPathFile = path;
}

void Light::SetOBJPathFile( const char *path ){
   this->OBJPathFile = path;
}

void Light::SetPath( std::string path_obj ){
   this->OBJPathFile = path_obj;
}

void Light::SetPath( const char *path_obj ){
   this->OBJPathFile = path_obj;
}

void Light::SetColor( glm::vec3 &color ){
   this->Color = color;
}

void Light::Load(){
   SDL_Log( "\n" );
   this->Init = LoadAssimp( this->OBJPathFile.c_str(), this->Vertices, this->Uvs, this->Normals, this->Indices );
   this->BindVAO();
}

void Light::BindVAO(){
   if( this->Init ){
      SDL_Log( "Binding %s into VAO\n", this->OBJPathFile.c_str() );
      glGenVertexArrays( 1, &this->VAO );
      glGenBuffers( 1, &this->VertexBuffer );
      glGenBuffers( 1, &this->IndicesBuffer );

      //Vertex:
      glBindBuffer( GL_ARRAY_BUFFER, this->VertexBuffer );
      glBufferData( GL_ARRAY_BUFFER, this->Vertices.size() * sizeof( glm::vec3 ), &this->Vertices[0], GL_STATIC_DRAW );

      //VAO:
      glBindVertexArray( this->VAO );

      //Vertex:
      glBindBuffer( GL_ARRAY_BUFFER, this->VertexBuffer );
      glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0 );
      glEnableVertexAttribArray( 0 );
      //Indicies:
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->IndicesBuffer );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(GLuint), &this->Indices[0], GL_STATIC_DRAW );

      glBindVertexArray( 0 );

      SDL_Log( "Binded %s into VAO\n", this->OBJPathFile.c_str() );
   }
   else{
      SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Before binding, load file and texture!\n" );
   }
}

void Light::Draw(){
   glUniformMatrix4fv( *Light::ModelUniformLight, 1, GL_FALSE, glm::value_ptr( this->ModelMatrix ) );
   glUniform3fv( *Light::UniformColorLight, 1, glm::value_ptr( this->Color ) );
   //Bind VAO:
   glBindVertexArray( this->VAO );
   //Draw:
   glDrawElements( GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, (GLvoid *)0 );
   //Unbind VAO:
   glBindVertexArray( 0 );

}

glm::vec3 Light::ReturnPosition(){
   return glm::vec3( this->ModelMatrix[3][0], this->ModelMatrix[3][1], this->ModelMatrix[3][2] );
}

void Light::Translate( glm::vec3 &in ){
   this->ModelMatrix = glm::translate( this->ModelMatrix, in );
}

void Light::Rotate( GLfloat angle, glm::vec3 &in ){
   this->ModelMatrix = glm::rotate( this->ModelMatrix, glm::radians( angle ), in );
}

void Light::Scale( glm::vec3 &in ){
   this->ModelMatrix = glm::scale( this->ModelMatrix, in );
}

void Light::ChangePosition( glm::vec3 &in ){
   this->ModelMatrix[3][0] = in.x;
   this->ModelMatrix[3][1] = in.y;
   this->ModelMatrix[3][2] = in.z;
}

void Light::Log() const{
   SDL_Log( "\tPosition: %f %f %f\n\tColor: %f %f %f\n",
            this->ModelMatrix[3][0],
            this->ModelMatrix[3][1],
            this->ModelMatrix[3][2],
            this->Color.x,
            this->Color.y,
            this->Color.z
   );
}
