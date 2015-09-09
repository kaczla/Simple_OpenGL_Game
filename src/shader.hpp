/*!
   \file shader.hpp
   \brief Plik odpowiedzialny za ładowanie shaderów.
*/

#ifndef shader_hpp
#define shader_hpp
#include <GL/glew.h>

/*!
   \brief Ładuje shader fragmentu i werzchołków do pamięci.

   \param vertex_shader_path_file - ścieżka do pliku z shaderem wierzchołków
   \param fragment_shader_path_file - ścieżka do pliku z shaderem fragmentu
   \return - identyfikator programu z dołączonymi shaderami
*/
GLuint LoadShader( const char* vertex_shader_path_file,
   const char* fragment_shader_path_file
);

#endif
