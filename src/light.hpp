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
   void Load();
   void BindVAO();
   void Draw();
   void Translate( glm::vec3 &in );
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
   //Color light object:
   glm::vec3 Color;
   //Check:
   bool Init = false;
};

#endif
