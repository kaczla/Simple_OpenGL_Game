/*!
   \file imgloader.hpp
   \brief Plik odpowiedzialny ładowanie tekstur dla obiektów.
*/
#ifndef imgloader_hpp
#define imgloader_hpp
#include <GL/glew.h>

/*!
   \brief Ładuje teksturę obiektu do pamięci.

   \param img_path_file - ścieżka do pliku z teksturą obiektu
   \param image - identyfikator tekstury obiektu
   \return - wartość logiczną dla ładowania tekstury, FALSE = błąd

   Wykorzystuje bibliotekę DevIL.
*/
bool LoadImg( const char *img_path_file, GLuint &image );

/*!
   \brief Ładuje teksturę obiektu do pamięci.

   \param img_path_file - ścieżka do pliku z teksturą obiektu
   \return - identyfikator tekstury obiektu

   Wykorzystuje bibliotekę DevIL.
*/
GLuint LoadImg( const char *img_path_file );

#endif
