#ifndef model_hpp
#define model_hpp
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Model{
public:
   Model();
   Model( std::string path_obj, std::string path_img );
   Model( const Model &model );
   Model & operator=( const Model &model );
   ~Model();
   void SetName( std::string &in );
   void SetOBJPathFile( std::string path );
   void SetOBJPathFile( const char *path );
   void SetImgPathFile( std::string path );
   void SetImgPathFile( const char *path );
   void SetImgSpecPathFile( const char* path );
   void SetImgSpecPathFile( std::string path );
   void SetMTLPathFile( std::string path );
   void SetMTLPathFile( const char *path );
   void SetPath( std::string path_obj, std::string path_img );
   void SetPath( const char *path_obj, const char *path_img );
   void SetAmbient( glm::vec3 &in );
   void SetDiffuse( glm::vec3 &in );
   void SetSpecular( glm::vec3 &in );
   void SetShininess( float &in );
   void Load_OBJ();
   void Load_Img();
   void Load();
   void BindVAO();
   void BindTexture();
   void UnbindTexture();
   void Draw();
   void DrawNoTexture();
   void DrawCollisionSquare();
   GLuint ReturnTexture();
   void Translate( glm::vec3 &in );
   void Translate( unsigned int i, glm::vec3 &in );
   void Rotate( GLfloat angle, glm::vec3 &in );
   void Rotate( unsigned int i, GLfloat angle, glm::vec3 &in );
   void Scale( glm::vec3 &in );
   void Scale( unsigned int i, glm::vec3 &in );
   void AddMatrix();
   void AddMatrix( glm::vec3 &in );
   void AddMatrix( glm::mat4 &in );
   //Uniforms:
   static GLuint * ModelUniformId;
   static GLuint * TextureUniformId;
   static GLuint * TextureSpecularUniformId;
   static GLuint * AmbientUniformId;
   static GLuint * DiffuseUniformId;
   static GLuint * SpecularUniformId;
   static GLuint * ShininessUniformId;
   //for collision:
   static GLuint * ModelUniformLight;
   static GLuint * UniformColorLight;
private:
   std::string Name;
   //VAO:
   GLuint VAO;
   //Data file:
   std::vector <glm::vec3> Vertices;
   std::vector <glm::vec2> Uvs;
   std::vector <glm::vec3> Normals;
   std::vector <GLuint> Indices;
   //Buffer:
   GLuint VertexBuffer;
   GLuint UvBuffer;
   GLuint NormalBuffer;
   GLuint IndicesBuffer;
   //Texture:
   GLuint Texture;
   GLuint TextureSpecular;
   //String path files:
   std::string OBJPathFile;
   std::string ImgPathFile;
   std::string ImgSpecPathFile;
   std::string MTLPathFile;
   //Global position model:
   std::vector <glm::mat4> ModelMatrix;
   std::vector <glm::mat4>::iterator It;
   //for light:
   glm::vec3 Ambient = glm::vec3( 0.2f );
   glm::vec3 Diffuse = glm::vec3( 0.5f );
   glm::vec3 Specular = glm::vec3( 0.5f );
   GLfloat Shininess = 32.0f;
   //Collision:
   void SetCollision();
   glm::vec3 CollisionMin, CollisionMax;
   std::vector <glm::vec3> CollisionSquare;
   GLuint CollisionSquareVao;
   GLuint CollisionSquareVertexBuffer;
   glm::vec3 CollisionColor = glm::vec3( 1.0f );
   //Check:
   bool Init = false;
};

#endif
