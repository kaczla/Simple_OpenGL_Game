#ifndef objloader_hpp
#define objloader_hpp
#include <glm/glm.hpp>
#include <vector>

bool LoadOBJ( const char *obj_path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals
);

#endif
