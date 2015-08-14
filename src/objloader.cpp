#include "objloader.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>

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
      std::vector <glm::vec3>::iterator it_vec3;
      std::vector <glm::vec3>::iterator it_vec2;
      std::vector <unsigned int>::iterator it_uint;

      for( it_uint = vertices_indices.begin(); it_uint != vertices_indices.end(); ++it_uint ){
         tmp_indices = *it_uint;
         vertices.push_back( vertices_tmp.at( tmp_indices -1 ) );
      }
      for( it_uint = uvs_indices.begin(); it_uint != uvs_indices.end(); ++it_uint ){
         tmp_indices = *it_uint;
         uvs.push_back( uvs_tmp.at( tmp_indices -1 ) );
      }
      for( it_uint = normals_indices.begin(); it_uint != normals_indices.end(); ++it_uint ){
         tmp_indices = *it_uint;
         normals.push_back( normals_tmp.at( tmp_indices -1 ) );
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
