#include "objloader.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <cstring>
#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool LoadOBJ( const char* obj_path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals
){
   vertices.clear();
   uvs.clear();
   normals.clear();
   SDL_Log( "Loading OBJ: %s\n", obj_path_file );
   std::ifstream OBJStream( obj_path_file, std::ios::in );
   if( OBJStream.good() ){
      std::vector <unsigned int> vertices_indices, uvs_indices, normals_indices;
      unsigned int tmp_indices;
      std::vector <glm::vec3> vertices_tmp;
      std::vector <glm::vec2> uvs_tmp;
      std::vector <glm::vec3> normals_tmp;
      std::string Line;
      std::string::iterator it;
      glm::vec3 vertex, normal;
      glm::vec2 uv;
      std::istringstream iss;
      while( getline( OBJStream ,Line ) ){
         if( Line.substr( 0, 2 ) == "v " ){
            Line = Line.substr( 2 );
            iss.str( "" );
            iss.clear();
            iss.str( Line );
            iss>>vertex.x;
            iss>>vertex.y;
            iss>>vertex.z;
            vertices_tmp.push_back( vertex );
         }
         else if( Line.substr( 0, 2 ) == "vt" ){
            Line = Line.substr( 3 );
            iss.str( "" );
            iss.clear();
            iss.str( Line );
            iss>>uv.x;
            iss>>uv.y;
            uvs_tmp.push_back( uv );
         }
         else if( Line.substr( 0, 2 ) == "vn" ){
            Line = Line.substr( 3 );
            iss.str( "" );
            iss.clear();
            iss.str( Line );
            iss>>normal.x;
            iss>>normal.y;
            iss>>normal.z;
            normals_tmp.push_back( normal );
         }
         else if( Line.substr( 0, 2 ) == "f " ){
            iss.str( "" );
            iss.clear();
            Line = Line.substr( 2 );
            for( it = Line.begin(); it != Line.end(); ++it ){
               if( *it == '/' ){
                  Line.replace( it, it+1, " " );
               }
            }
            iss.str( Line );
            Line.clear();

            iss>>tmp_indices;
            vertices_indices.push_back( tmp_indices );
            iss>>tmp_indices;
            uvs_indices.push_back( tmp_indices );
            iss>>tmp_indices;
            normals_indices.push_back( tmp_indices );

            iss>>tmp_indices;
            vertices_indices.push_back( tmp_indices );
            iss>>tmp_indices;
            uvs_indices.push_back( tmp_indices );
            iss>>tmp_indices;
            normals_indices.push_back( tmp_indices );

            iss>>tmp_indices;
            vertices_indices.push_back( tmp_indices );
            iss>>tmp_indices;
            uvs_indices.push_back( tmp_indices );
            iss>>tmp_indices;
            normals_indices.push_back( tmp_indices );
         }
         else if( Line.front() == '#' ){
            //ignore comment
         }
         else{
            //SDL_Log( "Unknown input: %s\n", Line.c_str() );
         }
      }
      OBJStream.close();
      bool print_error = true;
      std::vector <glm::vec3>::iterator it_vec3;
      std::vector <glm::vec3>::iterator it_vec2;
      std::vector <unsigned int>::iterator it_uint;

      for( it_uint = vertices_indices.begin(); it_uint != vertices_indices.end(); ++it_uint ){
         tmp_indices = *it_uint;
         if( tmp_indices > 0 and tmp_indices and
             tmp_indices < vertices_indices.size()
          ){
            vertices.push_back( vertices_tmp.at( tmp_indices -1 ) );
         }
         else if( print_error ){
            SDL_Log( "Something wrong with: indices of vertices\n" );
            print_error = false;
         }
      }
      print_error = true;
      for( it_uint = uvs_indices.begin(); it_uint != uvs_indices.end(); ++it_uint ){
         tmp_indices = *it_uint;
         if( tmp_indices > 0 and tmp_indices and
             tmp_indices < vertices_indices.size()
         ){
            uvs.push_back( uvs_tmp.at( tmp_indices -1 ) );
         }
         else if( print_error ){
            SDL_Log( "Something wrong with: indices of uvs\n" );
            print_error = false;
         }
      }
      print_error = true;
      for( it_uint = normals_indices.begin(); it_uint != normals_indices.end(); ++it_uint ){
         tmp_indices = *it_uint;
         if( tmp_indices > 0 and tmp_indices and
             tmp_indices < vertices_indices.size()
         ){
            normals.push_back( normals_tmp.at( tmp_indices -1 ) );
         }
         else if( print_error ){
            SDL_Log( "Something wrong with: indices of normals\n" );
            print_error = false;
         }
      }
   }
   else{
      SDL_Log( "Can't find file: %s\n", obj_path_file );
      return false;
   }
   SDL_Log( "vertex:%u   uv:%u   normal:%u\n", vertices.size(), uvs.size(), normals.size() );
   SDL_Log( "Loaded OBJ: %s\n", obj_path_file );
   return true;
}

struct Packe{
   glm::vec3 position;
   glm::vec2 uv;
   glm::vec3 normal;
   bool operator<(const Packe that) const{
      return memcmp( (void*)this, (void*)&that, sizeof( Packe ) ) > 0;
   };
};

bool GetSimilarVertexIndex(
   Packe &packed,
   std::map <Packe, GLuint> &VertexToOutIndex,
   GLuint &result
){
   std::map <Packe, GLuint>::iterator it = VertexToOutIndex.find( packed );
   if ( it == VertexToOutIndex.end() ){
      return false;
   }
   else{
      result = it->second;
      return true;
   }
}

void IndexVBO( std::vector <glm::vec3> &in_vertices,
   std::vector <glm::vec2> &in_uvs,
   std::vector <glm::vec3> &in_normals,
   std::vector <GLuint> &out_indices,
   std::vector <glm::vec3> &out_vertices,
   std::vector <glm::vec2> &out_uvs,
   std::vector <glm::vec3> &out_normals
){
   std::map <Packe, GLuint> VertexToOutIndex;
   unsigned int i = 0;
   GLuint index, newindex;
   for ( i = 0; i < in_vertices.size(); ++i ){
      Packe packed = {
         in_vertices[i],
         in_uvs[i],
         in_normals[i]
      };
      bool found = GetSimilarVertexIndex( packed, VertexToOutIndex, index );
      if ( found ){
         out_indices.push_back( index );
      }
      else{
         out_vertices.push_back( in_vertices[i] );
         out_uvs.push_back( in_uvs[i] );
         out_normals.push_back( in_normals[i] );
         newindex = (GLuint)out_vertices.size() - 1;
         out_indices.push_back( newindex );
         VertexToOutIndex[packed] = newindex;
      }
   }
}

bool LoadAssimp( const char *path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals,
   std::vector <GLuint> &indices
){
   vertices.clear();
   uvs.clear();
   normals.clear();
   indices.clear();
   SDL_Log( "Loading file: %s\n", path_file );
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile( path_file, 0 );
   if( !scene ){
      SDL_Log( "importer.ReadFile: %s\n", importer.GetErrorString() );
      return false;
   }
   const aiMesh* mesh = scene->mMeshes[0];
   unsigned int i = 0;
   aiVector3D tmp;
   //vertices:
   vertices.reserve( mesh->mNumVertices );
   for( i = 0; i < mesh->mNumVertices; ++i ){
      tmp = mesh->mVertices[i];
      vertices.push_back( glm::vec3( tmp.x, tmp.y, tmp.z ) );
   }
   //uvs:
   uvs.reserve( mesh->mNumVertices );
   for( i = 0; i < mesh->mNumVertices; ++i ){
      tmp = mesh->mTextureCoords[0][i];
      uvs.push_back( glm::vec2( tmp.x, tmp.y ) );
   }
   //normals:
   normals.reserve( mesh->mNumVertices );
   for( i = 0; i < mesh->mNumVertices; ++i ){
      tmp = mesh->mNormals[i];
      normals.push_back( glm::vec3( tmp.x, tmp.y, tmp.z ) );
   }
   //indices:
   indices.reserve( 3*mesh->mNumFaces );
   for( i = 0; i < mesh->mNumFaces; ++i ){
      indices.push_back( mesh->mFaces[i].mIndices[0] );
      indices.push_back( mesh->mFaces[i].mIndices[1] );
      indices.push_back( mesh->mFaces[i].mIndices[2] );
   }
   SDL_Log( "vertex:%u   uv:%u   normal:%u   indices:%u\n", vertices.size(), uvs.size(), normals.size(), indices.size() );
   SDL_Log( "Loaded file: %s\n", path_file );
   return true;
}
