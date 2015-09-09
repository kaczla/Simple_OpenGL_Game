/*!
   \file objloader.hpp
   \brief Plik odpowiedzialny za ładowanie plików .obj oraz .mtl.
*/
#ifndef objloader_hpp
#define objloader_hpp
#include <glm/glm.hpp>
#include <vector>

/*!
   \brief Ładuje plik .obj do pamięci.

   \param obj_path_file - ścieżka do pliku .obj
   \param vertices - wektor Wierzchołków
   \param uvs - wektor UV Map
   \param normals - wektor Normalnych
   \return - wartość logiczną dla ładowania pliku .obj, FALSE = błąd

   Nie wykorzystuje zewnętrznych bibliotek.
*/
bool LoadOBJ( const char *obj_path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals
);

/*!
   \brief Ładuje plik .obj do pamięci.

   \param path_file - ścieżka do pliku .obj
   \param vertices - wektor Wierzchołków
   \param uvs - wektor UV Map
   \param normals - wektor Normalnych
   \param indices - wektor Indeksów Wierzchołków
   \return - wartość logiczną dla ładowania pliku .obj, FALSE = błąd

   Wczytuje z plikuWykorzystuje bibliotekę assimp.
*/
bool LoadAssimp( const char *path_file,
   std::vector <glm::vec3> &vertices,
   std::vector <glm::vec2> &uvs,
   std::vector <glm::vec3> &normals,
   std::vector <GLuint> &indices
);

/*!
   \brief Tworzy Indeks Wierzchołków

   \param in_vertices - wektor Wierzchołków wejściowych
   \param in_uvs - wektor UV Map wejściowych
   \param in_normals - wektor Normalnych wejściowych
   \param out_indices - wektor Indeksów Wierzchołków wyjściowy
   \param out_vertices - wektor Wierzchołków wyjściowy
   \param out_uvs - wektor UV Map wyjściowy
   \param out_normals - wektor Normalnych wyjściowy

   Nie wykorzystuje zewnętrznych bibliotek.\n
   Wykorzystać, gdy nie otrzymano z pliku Indeksów Wierzchołków.\n
*/
void IndexVBO( std::vector <glm::vec3> &in_vertices,
   std::vector <glm::vec2> &in_uvs,
   std::vector <glm::vec3> &in_normals,
   std::vector <GLuint> &out_indices,
   std::vector <glm::vec3> &out_vertices,
   std::vector <glm::vec2> &out_uvs,
   std::vector <glm::vec3> &out_normals
);

/*!
   \brief Ładuje plik .mtl do pamięci.

   \param path_file - ścieżka do pliku .mtl
   \param ambient - wartość Ambient
   \param diffuse - wartość Diffuse
   \param specular - wartość Specular
   \param shininess - wartość Shininess (jakość odbicia)

   Nie wykorzystuje zewnętrznych bibliotek.\n
*/
void LoadMTL( const char *path_file,
   glm::vec3 &ambient,
   glm::vec3 &diffuse,
   glm::vec3 &specular,
   GLfloat &shininess
);

#endif
