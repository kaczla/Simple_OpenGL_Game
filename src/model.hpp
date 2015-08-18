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
   ~Model();
   void SetOBJPathFile( std::string path );
   void SetOBJPathFile( const char *path );
   void SetImgPathFile( std::string path );
   void SetImgPathFile( const char *path );
   void SetPath( std::string path_obj, std::string path_img );
   void SetPath( const char *path_obj, const char *path_img );
   void Load_OBJ();
   void Load_Img();
   void Load();
   void Bind();
private:
   GLuint ID;
   std::vector <glm::vec3> Vertices;
   std::vector <glm::vec2> Uvs;
   std::vector <glm::vec3> Normals;

   GLuint VertexBuffer;
   GLuint UvBuffer;
   GLuint NormalBuffer;

   GLuint Texture;

   std::string OBJPathFile;
   std::string ImgPathFile;
};

#endif
