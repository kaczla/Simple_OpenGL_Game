/*!
   \file model.hpp
   \brief Plik odpowiedzialny zarządzanie modelem obiektu.
*/
#ifndef model_hpp
#define model_hpp
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

/*!
   \brief Klasa odpowiedzialna za zarządzaniem modelem obiektu.
*/
class Model{
public:
   /*!
      \brief Konstruktor domyślny.

      Ustala podstawowe wartości dla obiektu.
   */
   Model();
   /*!
      \brief Konstruktor.

      \param path_obj - ścieżka do pliku .obj
      \param path_img - ścieżka do głównej tekstury

      Ustala podstawowe wartości dla obiektu.\n
      Ścieżka do spektralnej tekstury jest taka sama jak tekstury głównej.\n
      Załadowanie danych do pamięcie ( \link Load() \endlink ).
   */
   Model( std::string path_obj, std::string path_img );
   /*!
      \brief Konstruktor kopiujący.
   */
   Model( const Model &model );
   /*!
      \brief Operator przypisania.
   */
   Model & operator=( const Model &model );
   /*!
      \brief Destruktor.

      Zwalnia pamięć zaalokowanych elementów w OpenGL.
   */
   ~Model();
   /*!
      \brief Ustala nazwe obiektu.

      \param in - nowa nazwa dla obiwktu ( \link Name \endlink )
   */
   void SetName( std::string &in );
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
      \brief Ustala nową ścieżkę dla głównej tekstury.

      \param path - nowa wartość dla zmiennej \link ImgPathFile \endlink
   */
   void SetImgPathFile( std::string path );
   /*!
      \brief Ustala nową ścieżkę dla głównej tekstury.

      \param path - nowa wartość dla zmiennej \link ImgPathFile \endlink
   */
   void SetImgPathFile( const char *path );
   /*!
      \brief Ustala nową ścieżkę dla spektralnej tekstury.

      \param path - nowa wartość dla zmiennej \link ImgSpecPathFile \endlink
   */
   void SetImgSpecPathFile( const char* path );
   /*!
      \brief Ustala nową ścieżkę dla spektralnej tekstury.

      \param path - nowa wartość dla zmiennej \link ImgSpecPathFile \endlink
   */
   void SetImgSpecPathFile( std::string path );
   /*!
      \brief Ustala nową ścieżkę dla pliku .mtl.

      \param path - nowa wartość dla zmiennej \link MTLPathFile \endlink
   */
   void SetMTLPathFile( std::string path );
   /*!
      \brief Ustala nową ścieżkę dla pliku .mtl.

      \param path - nowa wartość dla zmiennej \link MTLPathFile \endlink
   */
   void SetMTLPathFile( const char *path );
   /*!
      \brief Ustala nową ścieżkę dla pliku .obj i głównej tekstury.

      \param path_obj - nowa wartość dla zmiennej \link OBJPathFile \endlink
      \param path_img - nowa wartość dla zmiennej \link ImgPathFile \endlink

      Ścieżka do spektralnej tekstury jest taka sama jak tekstury głównej.\n
   */
   void SetPath( std::string path_obj, std::string path_img );
   /*!
      \brief Ustala nową ścieżkę dla pliku .obj i głównej tekstury.

      \param path_obj - nowa wartość dla zmiennej \link OBJPathFile \endlink
      \param path_img - nowa wartość dla zmiennej \link ImgPathFile \endlink

      Ścieżka do spektralnej tekstury jest taka sama jak tekstury głównej.\n
   */
   void SetPath( const char *path_obj, const char *path_img );
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
      \brief Ustala nową wartość dla \link Shininess \endlink (jakość odbicia).

      \param in - nowa wartość dla zmiennej \link Shininess \endlink (jakość odbicia)
   */
   void SetShininess( float &in );
   /*!
      \brief Wczytuje dane obiektu z pliku .obj oraz .mtl. Ustala granice/kolizje obiektu.

      W razie błędu \link Init \endlink = FALSE.\n
   */
   void Load_OBJ();
   /*!
      \brief Wczytuje teksturę główną i spektralną dla obiektu.
   */
   void Load_Img();
   /*!
      \brief Wczytuje dane obiektu z pliku .obj, .mtl oraz teksturę główną i spektralną. Ustala granice/kolizje obiektu. Tworzy VAO (Vertex Array Object).

      W razie błędu \link Init \endlink = FALSE.\n
   */
   void Load();
   /*!
      \brief Tworzy VAO (Vertex Array Object).
   */
   void BindVAO();
   /*!
      \brief Aktywuje teksturę główną i spektralną.
   */
   void BindTexture();
   /*!
      \brief Deaktywuje teksturę główną i spektralną.
   */
   void UnbindTexture();
   /*!
      \brief Rysuje wszystkie obiekty.

      Przekazuje informacje do shaderów, aktywuje teksturę główną i spektralną oraz rysuje wszystkie obiekty.
   */
   void Draw();
   /*!
      \brief Rysuje wszystkie obiekty bez aktywowania tekstur.

      Przekazuje informacje do shaderów oraz rysuje wszystkie obiekty.
   */
   void DrawNoTexture();
   /*!
      \brief Rysuje granice/kolizje wszystkich obiektów.

      Przekazuje informacje do shaderów oraz rysuje granice/kolizje wszystkich obiektów.
   */
   void DrawCollisionSquare();
   /*!
      \brief Zwraca identyfikator głównej tekstury ( \link Texture \endlink ).
   */
   GLuint ReturnTexture();
   /*!
      \brief Zwraca identyfikator spektralnej tekstury ( \link TextureSpecular \endlink ).
   */
   GLuint ReturnTextureSpecular();
   /*!
      \brief Translacja wszystkich macierzy modelu ( \link ModelMatrix \endlink ).

      \param in - wartość o jaką zostaną przesuniete macierze modelu ( \link ModelMatrix \endlink )
   */
   void Translate( glm::vec3 &in );
   /*!
      \brief Translacja i-tej macierzy modelu ( \link ModelMatrix \endlink ).

      \param i - numer macierzy, która będzie przesunięta
      \param in - wartość o jaką zostanie przesunieta i-ta macierz modelu ( \link ModelMatrix \endlink )
   */
   void Translate( unsigned int i, glm::vec3 &in );
   /*!
      \brief Rotacja wszystkich macierzy modelu ( \link ModelMatrix \endlink ).

      \param angle - wartość kąta w stopniach o jaki zostaną obrócone wszystkie macierze modelu ( \link ModelMatrix \endlink )
      \param in - wartość wektora o jaki zostaną obrócone wszystkie macierz modelu ( \link ModelMatrix \endlink )
   */
   void Rotate( GLfloat angle, glm::vec3 &in );
   /*!
      \brief Rotacja i-tej macierzy modelu ( \link ModelMatrix \endlink ).

      \param i - numer macierzy, która będzie obrócona
      \param angle - wartość kąta w stopniach o jaki zostanie obrócona i-ta macierz modelu ( \link ModelMatrix \endlink )
      \param in - wartość wektora o jaki zostanie obrócona i-ta macierz modelu ( \link ModelMatrix \endlink )
   */
   void Rotate( unsigned int i, GLfloat angle, glm::vec3 &in );
   /*!
      \brief Skalowanie wszystkich macierzy modelu ( \link ModelMatrix \endlink ).

      \param in - wartość o jaką zostaną przeskalowane wszystkie macierz modelu ( \link ModelMatrix \endlink )
   */
   void Scale( glm::vec3 &in );
   /*!
      \brief Skalowanie i-tej macierzy modelu ( \link ModelMatrix \endlink ).

      \param i - numer macierzy, która będzie przeskalowana
      \param in - wartość o jaką zostanie przeskalowana i-ta macierz modelu ( \link ModelMatrix \endlink )
   */
   void Scale( unsigned int i, glm::vec3 &in );
   /*!
      \brief Dodanie domyślnej macierzy modelu do wektora \link ModelMatrix \endlink.
   */
   void AddMatrix();
   /*!
      \brief Dodanie macierzy modelu z przesunięciem do wszystkiech macierzy modeli ( \link ModelMatrix \endlink ).

      \param in - wektor przesunięcia dla macierzy modelu
   */
   void AddMatrix( glm::vec3 &in );
   /*!
      \brief Dodanie macierzy modelu do wszystkiech macierzy modeli ( \link ModelMatrix \endlink ).

      \param in - macierz modelu, która zostanie dodana do wszystkich macierzy modeli
   */
   void AddMatrix( glm::mat4 &in );
   /*!
      \brief Translacja i-tej macierzy modelu ( \link ModelMatrix \endlink ).

      \param i - numer macierzy, która będzie przesunięta
      \param in - wartość o jaką zostanie przesunieta i-ta macierz modelu ( \link ModelMatrix \endlink )
   */
   void ChangeMatrix( unsigned int i, glm::vec3 &in );
   /*!
      \brief Zastąpienie i-tej macierzy modelu ( \link ModelMatrix \endlink ).

      \param i - numer macierzy, która będzie zastąpiony
      \param in - wartość macierzy, która będzie zastąpiona z i-tą macierzą modelu ( \link ModelMatrix \endlink )
   */
   void ChangeMatrix( unsigned int i, glm::mat4 &in );
   //Uniforms:
   /*!
      \brief Wskaźnik do uniformu macierzy modelu.
   */
   static GLuint * ModelUniformId;
   /*!
      \brief Wskaźnik do uniformu głównej tekstury.
   */
   static GLuint * TextureUniformId;
   /*!
      \brief Wskaźnik do uniformu spektralnej tekstury.
   */
   static GLuint * TextureSpecularUniformId;
   /*!
      \brief Wskaźnik do uniformu wartości Ambient.
   */
   static GLuint * AmbientUniformId;
   /*!
      \brief Wskaźnik do uniformu wartości Diffuse.
   */
   static GLuint * DiffuseUniformId;
   /*!
      \brief Wskaźnik do uniformu wartości Specular.
   */
   static GLuint * SpecularUniformId;
   /*!
      \brief Wskaźnik do uniformu wartości Shininess (jakość odbicia).
   */
   static GLuint * ShininessUniformId;
   //for collision:
   /*!
      \brief Wskaźnik do uniformu granicy/kolizji modelu.
   */
   static GLuint * ModelUniformLight;
   /*!
      \brief Wskaźnik do uniformu koloru granicy/kolizji.
   */
   static GLuint * UniformColorLight;
private:
   /*!
      \brief Nazwa obiektu.
   */
   std::string Name;
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
      \brief Identyfikator UV Map.
   */
   GLuint UvBuffer;
   /*!
      \brief Identyfikator Normalnych.
   */
   GLuint NormalBuffer;
   /*!
      \brief Identyfikator Indeksów Wierzchołków.
   */
   GLuint IndicesBuffer;
   //Texture:
   /*!
      \brief Identyfikator głównej tekstury.
   */
   GLuint Texture;
   /*!
      \brief Identyfikator spektralnej tekstury.
   */
   GLuint TextureSpecular;
   //String path files:
   /*!
      \brief Ścieżka do pliku .obj.
   */
   std::string OBJPathFile;
   /*!
      \brief Ścieżka do pliku głównej tekstury.
   */
   std::string ImgPathFile;
   /*!
      \brief Ścieżka do pliku spektralnej tekstury.
   */
   std::string ImgSpecPathFile;
   /*!
      \brief Ścieżka do pliku .mtl.
   */
   std::string MTLPathFile;
   //Global position model:
   /*!
      \brief Wrzystkie macierze modelu.
   */
   std::vector <glm::mat4> ModelMatrix;
   /*!
      \brief Iterator dla macierzy modelu.
   */
   std::vector <glm::mat4>::iterator It;
   //for light:
   /*!
      \brief Wartość Ambient.
   */
   glm::vec3 Ambient = glm::vec3( 0.2f );
   /*!
      \brief Wartość Diffuse.
   */
   glm::vec3 Diffuse = glm::vec3( 0.5f );
   /*!
      \brief Wartość Specular.
   */
   glm::vec3 Specular = glm::vec3( 0.5f );
   /*!
      \brief Wartość Shininess (jakość odbicia).
   */
   GLfloat Shininess = 32.0f;
   //Collision:
   /*!
      \brief Ustala granice obiektu dla wszystkich obiektów.
   */
   void SetCollision();
   /*!
      \var CollisionMin
      \brief Minimalna granica/kolizja obiektu.

   */
   /*!
      \var CollisionMax
      \brief Maksymalna granica/kolizja obiektu.
   */
   glm::vec3 CollisionMin, CollisionMax;
   /*!
      \brief Wektor Wierzchołków dla granicy/kolizji obiektu.
   */
   std::vector <glm::vec3> CollisionSquare;
   /*!
      \brief Identyfikator VAO (Vertex Array Object) dla granicy/kolizji obiektu.
   */
   GLuint CollisionSquareVao;
   /*!
      \brief Identyfikator Wierzchołków dla granicy/kolizji obiektu.
   */
   GLuint CollisionSquareVertexBuffer;
   /*!
      \brief Kolor dla granicy/kolizji obiektu.
   */
   glm::vec3 CollisionColor = glm::vec3( 1.0f );
   //Check:
   /*!
      \brief Poprawność zainicjalizowanych wszystkich elementów. FALSE = Błąd.
   */
   bool Init = false;
};

#endif
