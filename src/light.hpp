#ifndef light_hpp
#define light_hpp
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Light{
public:
   Light();
   Light( std::string path_obj );
   Light( const Light &light );
   ~Light();
   Light & operator=( const Light &light );
   void SetOBJPathFile( std::string path );
   void SetOBJPathFile( const char *path );
   void SetPath( std::string path_obj );
   void SetPath( const char *path_obj );
   void SetColor( glm::vec3 &color );
   void SetAmbient( glm::vec3 &in );
   void SetDiffuse( glm::vec3 &in );
   void SetSpecular( glm::vec3 &in );
   void Load();
   void BindVAO();
   void Draw();
   glm::vec3 ReturnPosition();
   glm::vec3 ReturnAmbient() const;
   glm::vec3 ReturnDiffuse() const;
   glm::vec3 ReturnSpecular() const;
   void Translate( glm::vec3 &in );
   void Rotate( GLfloat angle, glm::vec3 &in );
   void Scale( glm::vec3 &in );
   void ChangePosition( glm::vec3 &in );
   void Log() const;
   //Uniform:
   static GLuint * ModelUniformLight;
   static GLuint * UniformColorLight;
private:
   //VAO:
   GLuint VAO;
   //Data file:
   std::vector <glm::vec3> Vertices;
   std::vector <glm::vec2> Uvs;
   std::vector <glm::vec3> Normals;
   std::vector <GLuint> Indices;
   //Buffer:
   GLuint VertexBuffer;
   GLuint IndicesBuffer;
   //String path file:
   std::string OBJPathFile;
   //Position light:
   glm::mat4 ModelMatrix = glm::mat4( 1.0f );
   //Power light:
   glm::vec3 Ambient = glm::vec3( 0.5f );
   glm::vec3 Diffuse = glm::vec3( 0.5f );
   glm::vec3 Specular = glm::vec3( 0.5f );
   //Color light object:
   glm::vec3 Color;
   //Check:
   bool Init = false;
};

#endif
