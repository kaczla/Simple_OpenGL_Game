#include "model.hpp"
#include <SDL2/SDL.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objloader.cpp"
#include "imgloader.cpp"

GLuint * Model::ModelUniformId = NULL;
GLuint * Model::TextureUniformId = NULL;
GLuint * Model::TextureSpecularUniformId = NULL;
GLuint * Model::AmbientUniformId = NULL;
GLuint * Model::DiffuseUniformId = NULL;
GLuint * Model::SpecularUniformId = NULL;
GLuint * Model::ShininessUniformId = NULL;

Model::Model(){
   this->VAO = 0;
   this->VertexBuffer = 0;
   this->UvBuffer = 0;
   this->NormalBuffer = 0;
   this->IndicesBuffer = 0;
   this->Texture = 0;
}

Model::Model( std::string path_obj, std::string path_img ){
   this->VAO = 0;
   this->VertexBuffer = 0;
   this->UvBuffer = 0;
   this->NormalBuffer = 0;
   this->IndicesBuffer = 0;
   this->Texture = 0;

   this->OBJPathFile = path_obj;
   this->ImgPathFile = path_img;
   this->Load();
}

Model::Model( const Model &model ){
   this->VAO = model.VAO;

   this->Vertices = model.Vertices;
   this->Uvs = model.Uvs;
   this->Normals = model.Normals;
   this->Indices = model.Indices;

   this->VertexBuffer = model.VertexBuffer;
   this->UvBuffer = model.UvBuffer;
   this->NormalBuffer = model.NormalBuffer;
   this->IndicesBuffer = model.IndicesBuffer;

   this->Texture = model.Texture;
   this->TextureSpecular = model.TextureSpecular;

   this->OBJPathFile = model.OBJPathFile;
   this->ImgPathFile = model.ImgPathFile;
   this->ImgSpecPathFile = model.ImgSpecPathFile;
   this->MTLPathFile = model.MTLPathFile;

   this->Ambient = model.Ambient;
   this->Diffuse = model.Diffuse;
   this->Specular = model.Specular;
   this->Shininess = model.Shininess;

   this->ModelMatrix = model.ModelMatrix;
   this->Init = model.Init;
}

Model & Model::operator=( const Model &model ){
   this->VAO = model.VAO;

   this->Vertices = model.Vertices;
   this->Uvs = model.Uvs;
   this->Normals = model.Normals;
   this->Indices = model.Indices;

   this->VertexBuffer = model.VertexBuffer;
   this->UvBuffer = model.UvBuffer;
   this->NormalBuffer = model.NormalBuffer;
   this->IndicesBuffer = model.IndicesBuffer;

   this->Texture = model.Texture;
   this->TextureSpecular = model.TextureSpecular;

   this->OBJPathFile = model.OBJPathFile;
   this->ImgPathFile = model.ImgPathFile;
   this->ImgSpecPathFile = model.ImgSpecPathFile;
   this->MTLPathFile = model.MTLPathFile;

   this->Ambient = model.Ambient;
   this->Diffuse = model.Diffuse;
   this->Specular = model.Specular;
   this->Shininess = model.Shininess;

   this->ModelMatrix = model.ModelMatrix;
   this->Init = model.Init;

   return *this;
}

Model::~Model(){
   glDeleteTextures( 1, &this->Texture );
   glDeleteBuffers( 1, &this->VertexBuffer );
   glDeleteBuffers( 1, &this->UvBuffer );
   glDeleteBuffers( 1, &this->NormalBuffer );
   glDeleteBuffers( 1, &this->IndicesBuffer );
   glDeleteVertexArrays( 1, &this->VAO );
}

void Model::SetOBJPathFile( std::string path ){
   this->OBJPathFile = path;
}

void Model::SetOBJPathFile( const char *path ){
   this->OBJPathFile = path;
}

void Model::SetImgPathFile( std::string path ){
   this->ImgPathFile = path;
}

void Model::SetImgPathFile( const char *path ){
   this->ImgPathFile = path;
}

void Model::SetImgSpecPathFile( const char* path ){
   this->ImgSpecPathFile = path;
}

void Model::SetImgSpecPathFile( std::string path ){
   this->ImgSpecPathFile = path;
}

void Model::SetMTLPathFile( std::string path ){
   this->MTLPathFile = path;
}

void Model::SetMTLPathFile( const char *path ){
   this->MTLPathFile = path;
}

void Model::SetPath( std::string path_obj, std::string path_img ){
   this->OBJPathFile = path_obj;
   this->ImgPathFile = path_img;
}

void Model::SetPath( const char *path_obj, const char *path_img ){
   this->OBJPathFile = path_obj;
   this->ImgPathFile = path_img;
}

void Model::Load_OBJ(){
   SDL_Log( "\n" );
   /*
   //With LoadOBJ( ):
   this->Init = LoadOBJ( this->OBJPathFile.c_str(), this->Vertices, this->Uvs, this->Normals );
   std::vector <glm::vec3> Vertices_tmp;
   std::vector <glm::vec2> Uvs_tmp;
   std::vector <glm::vec3> Normals_tmp;
   IndexVBO( this->Vertices, this->Uvs, this->Normals, this->Indices, Vertices_tmp, Uvs_tmp, Normals_tmp );
   this->Vertices = Vertices_tmp;
   this->Uvs = Uvs_tmp;
   this->Normals = Normals_tmp;
   */
   //faster
   //with Assimp:
   this->Init = LoadAssimp( this->OBJPathFile.c_str(), this->Vertices, this->Uvs, this->Normals, this->Indices );
   LoadMTL( this->MTLPathFile.c_str(), this->Ambient, this->Diffuse, this->Specular, this->Shininess );
}

void Model::Load_Img(){
   this->Texture = LoadImg( this->ImgPathFile.c_str() );
   this->TextureSpecular = LoadImg( this->ImgSpecPathFile.c_str() );
}

void Model::Load(){
   this->Load_OBJ();
   this->Load_Img();
   this->BindVAO();
}

void Model::BindVAO(){
   if( this->Init ){
      SDL_Log( "Binding %s into VAO\n", this->OBJPathFile.c_str() );
      glGenVertexArrays( 1, &this->VAO );
      glGenBuffers( 1, &this->VertexBuffer );
      glGenBuffers( 1, &this->UvBuffer );
      glGenBuffers( 1, &this->NormalBuffer );
      glGenBuffers( 1, &this->IndicesBuffer );

      //Vertex:
      glBindBuffer( GL_ARRAY_BUFFER, this->VertexBuffer );
      glBufferData( GL_ARRAY_BUFFER, this->Vertices.size() * sizeof( glm::vec3 ), &this->Vertices[0], GL_STATIC_DRAW );
      //Uv:
      glBindBuffer( GL_ARRAY_BUFFER, this->UvBuffer );
      glBufferData( GL_ARRAY_BUFFER, this->Uvs.size() * sizeof( glm::vec2 ), &this->Uvs[0], GL_STATIC_DRAW );
      //Normal:
      glBindBuffer( GL_ARRAY_BUFFER, this->NormalBuffer );
      glBufferData( GL_ARRAY_BUFFER, this->Normals.size() * sizeof( glm::vec3 ), &this->Normals[0], GL_STATIC_DRAW );

      //VAO:
      glBindVertexArray( VAO );

      //Vertex:
      glBindBuffer( GL_ARRAY_BUFFER, this->VertexBuffer );
      glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0 );
      glEnableVertexAttribArray( 0 );
      //Uv:
      glBindBuffer( GL_ARRAY_BUFFER, this->UvBuffer );
      glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0 );
      glEnableVertexAttribArray( 1 );
      //Normal:
      glBindBuffer( GL_ARRAY_BUFFER, this->NormalBuffer );
      glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0 );
      glEnableVertexAttribArray( 2 );
      //Indicies:
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->IndicesBuffer );
      glBufferData( GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(GLuint), &this->Indices[0], GL_STATIC_DRAW );

      glBindVertexArray( 0 );

      SDL_Log( "Binded %s into VAO\n", this->OBJPathFile.c_str() );
   }
   else{
      SDL_Log( "Before binding, load file and texture!\n" );
   }
}

void Model::BindTexture(){
   glUniformMatrix4fv( *Model::ModelUniformId, 1, GL_FALSE, glm::value_ptr( this->ModelMatrix ) );
   glUniform3fv( *Model::AmbientUniformId, 1, glm::value_ptr( this->Ambient ) );
   glUniform3fv( *Model::DiffuseUniformId, 1, glm::value_ptr( this->Diffuse ) );
   glUniform3fv( *Model::SpecularUniformId, 1, glm::value_ptr( this->Specular ) );
   glUniform1f( *Model::ShininessUniformId, this->Shininess );

   glUniform1i( *Model::TextureUniformId, 0 );
   glUniform1i( *Model::TextureSpecularUniformId, 1 );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, this->Texture );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, this->TextureSpecular );
}

void Model::UnbindTexture(){
   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, 0 );
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, 0 );
}

void Model::Draw(){
   //Bind Texture into Uniform:
   glUniformMatrix4fv( *Model::ModelUniformId, 1, GL_FALSE, glm::value_ptr( this->ModelMatrix ) );
   glUniform3fv( *Model::AmbientUniformId, 1, glm::value_ptr( this->Ambient ) );
   glUniform3fv( *Model::DiffuseUniformId, 1, glm::value_ptr( this->Diffuse ) );
   glUniform3fv( *Model::SpecularUniformId, 1, glm::value_ptr( this->Specular ) );
   glUniform1f( *Model::ShininessUniformId, this->Shininess );

   glUniform1i( *Model::TextureUniformId, 0 );
   glUniform1i( *Model::TextureSpecularUniformId, 1 );

   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, this->Texture );

   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, this->TextureSpecular );

   //Bind VAO:
   glBindVertexArray( this->VAO );
   //Draw:
   glDrawElements( GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, (GLvoid *)0 );
   //Unbind VAO:
   glBindVertexArray( 0 );

   //Unbind Texture:
   glActiveTexture( GL_TEXTURE1 );
   glBindTexture( GL_TEXTURE_2D, 0 );
   glActiveTexture( GL_TEXTURE0 );
   glBindTexture( GL_TEXTURE_2D, 0 );
}

void Model::DrawNoTexture(){
   glBindVertexArray( this->VAO );
   glDrawElements( GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, (GLvoid *)0 );
   glBindVertexArray( 0 );
}

void Model::Translate( glm::vec3 &in ){
   this->ModelMatrix = glm::translate( this->ModelMatrix, in );
}

void Model::Rotate( GLfloat angle, glm::vec3 &in ){
   this->ModelMatrix = glm::rotate( this->ModelMatrix, glm::radians( angle ), in );
}

void Model::Scale( glm::vec3 &in ){
   this->ModelMatrix = glm::scale( this->ModelMatrix, in );
}
