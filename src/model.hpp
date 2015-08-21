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
   void SetOBJPathFile( std::string path );
   void SetOBJPathFile( const char *path );
   void SetImgPathFile( std::string path );
   void SetImgPathFile( const char *path );
   void SetPath( std::string path_obj, std::string path_img );
   void SetPath( const char *path_obj, const char *path_img );
   void Load_OBJ();
   void Load_Img();
   void Load();
   void BindVAO();
   void BindTexture();
   void UnbindTexture();
   void Draw();
   void DrawNoTexture();
   GLuint ReturnTexture();
   //Uniforms:
   static GLuint * ModelUniformId;
   static GLuint * TextureUniformId;
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
   GLuint UvBuffer;
   GLuint NormalBuffer;
   GLuint IndicesBuffer;
   //Texture:
   GLuint Texture;
   //String path files:
   std::string OBJPathFile;
   std::string ImgPathFile;
   //Global position model:
   glm::mat4 ModelMatrix = glm::mat4( 1.0f );
   //Check:
   bool Init = false;
};

#endif
