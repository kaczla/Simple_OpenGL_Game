#ifndef objloader_hpp
#define objloader_hpp
#include <glm/glm.hpp>
#include <vector>

bool LoadOBJ( const char *obj_path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals
);

bool LoadAssimp( const char *path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals,
   std::vector <GLuint> &indices
);

void IndexVBO( std::vector <glm::vec3> &in_vertices,
   std::vector <glm::vec2> &in_uvs,
   std::vector <glm::vec3> &in_normals,
   std::vector <GLuint> &out_indices,
   std::vector <glm::vec3> &out_vertices,
   std::vector <glm::vec2> &out_uvs,
   std::vector <glm::vec3> &out_normals
);

#endif
