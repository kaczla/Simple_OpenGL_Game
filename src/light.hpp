/*!
   \file light.hpp
   \brief Plik odpowiedzialny zarządzanie obiektem światła.
*/
#ifndef light_hpp
#define light_hpp
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

/*!
   \brief Klasa odpowiedzialna za zarządzaniem obiektem oświetlenia.
*/
class Light{
public:
   /*!
      \brief Konstruktor domyślny.

      Ustala podstawowe wartości dla obiektu.
   */
   Light();
   /*!
      \brief Konstruktor.

      \param path_obj - nowa wartość dla zmiennej \link OBJPathFile \endlink

      Ustala podstawowe wartości dla obiektu.
   */
   Light( std::string path_obj );
   /*!
      \brief Konstruktor kopiujący.
   */
   Light( const Light &light );
   /*!
      \brief Destruktor.

      Zwalnia pamięć zaalokowanych elementów w OpenGL.
   */
   ~Light();
   /*!
      \brief Operator przypisania.
   */
   Light & operator=( const Light &light );
   /*!
      \brief Ustala nową ścieżkę dla pliku .obj.

      \param path - nowa wartość dla zmiennej \link OBJPathFile \endlink
   */
   void SetOBJPathFile( std::string path );
   /*!
      \brief Ustala nową ścieżkę dla pliku .obj.

      \param path - nowa wartość dla zmiennej \link OBJPathFile \endlink
   */
   void SetOBJPathFile( const char *path );
   /*!
      \brief Ustala nową ścieżkę dla pliku .obj.

      \param path_obj - nowa wartość dla zmiennej \link OBJPathFile \endlink
   */
   void SetPath( std::string path_obj );
   /*!
      \brief Ustala nową ścieżkę dla pliku .obj.

      \param path_obj - nowa wartość dla zmiennej \link OBJPathFile \endlink
   */
   void SetPath( const char *path_obj );
   /*!
      \brief Ustala nowy kolor dla obiektu oświetlenia.

      \param color - nowa wartość dla zmiennej \link Color \endlink
   */
   void SetColor( glm::vec3 &color );
   /*!
      \brief Ustala nową wartość dla \link Ambient \endlink.

      \param in - nowa wartość dla zmiennej \link Ambient \endlink
   */
   void SetAmbient( glm::vec3 &in );
   /*!
      \brief Ustala nową wartość dla \link Diffuse \endlink.

      \param in - nowa wartość dla zmiennej \link Diffuse \endlink
   */
   void SetDiffuse( glm::vec3 &in );
   /*!
      \brief Ustala nową wartość dla \link Specular \endlink.

      \param in - nowa wartość dla zmiennej \link Specular \endlink
   */
   void SetSpecular( glm::vec3 &in );
   /*!
      \brief Wczytuje dane obiektu z pliku .obj.

      W razie błędu \link Init \endlink = FALSE.
   */
   void Load();
   /*!
      \brief Tworzy VAO (Vertex Array Object).
   */
   void BindVAO();
   /*!
      \brief Rysuje obiekt oświetlenia.

      Przekazuje informacje do shaderów oraz rysuje obiekt oświetlenia.
   */
   void Draw();
   /*!
      \brief Zwraca aktualną pozycję obiektu oświetlenia.
   */
   glm::vec3 ReturnPosition();
   /*!
      \brief Zwraca aktualną wartość zmiennej \link Ambient \endlink.
   */
   glm::vec3 ReturnAmbient() const;
   /*!
      \brief Zwraca aktualną wartość zmiennej \link Diffuse \endlink.
   */
   glm::vec3 ReturnDiffuse() const;
   /*!
      \brief Zwraca aktualną wartość zmiennej \link Specular \endlink.
   */
   glm::vec3 ReturnSpecular() const;
   /*!
      \brief Translacja macierzy modelu ( \link ModelMatrix \endlink ).

      \param in - wartość o jaką zostanie przesunieta macierz modelu ( \link ModelMatrix \endlink )
   */
   void Translate( glm::vec3 &in );
   /*!
      \brief Rotacja macierzy modelu ( \link ModelMatrix \endlink ).

      \param angle - wartość kąta w stopniach o jaki zostanie obrócona macierz modelu ( \link ModelMatrix \endlink )
      \param in - wartość wektora o jaki zostanie obrócona macierz modelu ( \link ModelMatrix \endlink )
   */
   void Rotate( GLfloat angle, glm::vec3 &in );
   /*!
      \brief Skalowanie macierzy modelu ( \link ModelMatrix \endlink ).

      \param in - wartość o jaką zostanie przeskalowana macierz modelu ( \link ModelMatrix \endlink )
   */
   void Scale( glm::vec3 &in );
   /*!
      \brief Zmiana aktualnej pozycji obiektu oświetlenia.

      \param in - wartość na jaką zostanie przeniesiony obiektu oświetlenia
   */
   void ChangePosition( glm::vec3 &in );
   /*!
      \brief Wyświetla informacje o oświetleniu.
   */
   void Log() const;
   //Uniform:
   /*!
      \brief Wskaźnik do uniformu macierzy modelu.
   */
   static GLuint * ModelUniformLight;
   /*!
      \brief Wskaźnik do uniformu koloru obiektu.
   */
   static GLuint * UniformColorLight;
private:
   //VAO:
   /*!
      \brief Identyfikator VAO (Vertex Array Object).
   */
   GLuint VAO;
   //Data file:
   /*!
      \brief Wektor Wierzchołków.
   */
   std::vector <glm::vec3> Vertices;
   /*!
      \brief Wektor UV Map.
   */
   std::vector <glm::vec2> Uvs;
   /*!
      \brief Wektor Normalnych.
   */
   std::vector <glm::vec3> Normals;
   /*!
      \brief Wektor Indeksów Wierzchołków.
   */
   std::vector <GLuint> Indices;
   //Buffer:
   /*!
      \brief Identyfikator Wierzchołków.
   */
   GLuint VertexBuffer;
   /*!
      \brief Identyfikator Indeksów Wierzchołków.
   */
   GLuint IndicesBuffer;
   //String path file:
   /*!
      \brief Ścieżka do pliku .obj.
   */
   std::string OBJPathFile;
   //Position light:
   /*!
      \brief Macierz modelu.
   */
   glm::mat4 ModelMatrix = glm::mat4( 1.0f );
   //Power light:
   /*!
      \brief Wartość Ambient.
   */
   glm::vec3 Ambient = glm::vec3( 0.5f );
   /*!
      \brief Wartość Diffuse.
   */
   glm::vec3 Diffuse = glm::vec3( 0.5f );
   /*!
      \brief Wartość Specular.
   */
   glm::vec3 Specular = glm::vec3( 0.5f );
   //Color light object:
   /*!
      \brief Kolor obiektu.
   */
   glm::vec3 Color;
   //Check:
   /*!
      \brief Poprawność zainicjalizowanych wszystkich elementów. FALSE = Błąd.
   */
   bool Init = false;
};

#endif
