/*!
   \file camera.hpp
   \brief Plik odpowiedzialny zarządzanie kamerą gracza.
*/
#ifndef camera_hpp
#define camera_hpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/detail/setup.hpp>
#if GLM_VERSION >= 96
#include <glm/gtc/vec1.hpp>
#else
#include <glm/gtx/vec1.hpp>
#endif

using namespace glm;

/*!
   \brief Klasa odpowiedzialna za zarządzaniem kamerą gracza.
*/
class Camera{
public:
   /*!
      \brief Konstruktor domyślny, ustawia macierz projekcji.
   */
   Camera();
   /*!
      \brief Konstruktor kopiujący.
   */
   Camera( const Camera &camera );
   /*!
      \brief Destruktor.
   */
   ~Camera();
   /*!
      \brief Operator przypisania.
   */
   Camera & operator=( const Camera &camera );
   /*!
      \brief Zwraca macierz widoku.
   */
   mat4 getViewMatrix() const;
   /*!
      \brief Zwraca macierz projekcji.
   */
   mat4 getProjectionMatrix() const;
   /*!
      \brief Ustala wartość \link VOF \endlink.

      \param vof - nowa wartość dla \link VOF \endlink
   */
   void SetVOF( vec1 &vof );
   /*!
      \brief Ustala wartość \link Aspect \endlink.

      \param aspect - nowa wartość dla \link Aspect \endlink
   */
   void SetAspect( vec1 &aspect );
   /*!
      \brief Ustala wartość \link Near \endlink.

      \param near - nowa wartość dla \link Near \endlink
   */
   void SetNear( vec1 &near );
   /*!
      \brief Ustala wartość \link Far \endlink.

      \param far - nowa wartość dla \link Far \endlink
   */
   void SetFar( vec1 &far );
   /*!
      \brief Ustala wartość \link PositionMin \endlink.

      \param min - nowa wartość dla \link PositionMin \endlink
   */
   void SetPositionMin( vec3 &min );
   /*!
      \brief Ustala wartość \link PositionMax \endlink.

      \param max - nowa wartość dla \link PositionMax \endlink
   */
   void SetPositionMax( vec3 &max );
   /*!
      \brief Ustala wartość dla \link PositionMin \endlink i \link PositionMax \endlink.

      \param min - nowa wartość dla \link PositionMin \endlink
      \param max - nowa wartość dla \link PositionMax \endlink
   */
   void SetPositionMinMax( vec3 &min, vec3 &max );
   /*!
      \brief Zwiększa lub zmniejsza wartość \link VOF \endlink.

      \param in - wartość która zostanie dodana do \link VOF \endlink
   */
   void ChangeVOF( glm::vec1 &in );
   /*!
      \brief Obrót kamery.

      \param Mouse - wartość przesunięcia dla \link ViewDirection \endlink
      
      Szybkość obrotu uzależniona jest od wielkości \link RotationSpeed \endlink.
   */
   void MouseUpdate( const vec2 &Mouse );
   /*!
      \brief Porusza kamerę w góry.

      Szybkość poruszania uzależniona jest od wielkości \link MovementSpeed \endlink.
   */
   void MoveUp();
   /*!
      \brief Porusza kamerę w dół.

      Szybkość poruszania uzależniona jest od wielkości \link MovementSpeed \endlink.
   */
   void MoveDown();
   /*!
      \brief Porusza kamerę do przodu (przed siebie).

      Szybkość poruszania uzależniona jest od wielkości \link MovementSpeed \endlink.
   */
   void MoveForward();
   /*!
      \brief Porusza kamerę do tyłu.

      Szybkość poruszania uzależniona jest od wielkości \link MovementSpeed \endlink.
   */
   void MoveBackward();
   /*!
      \brief Porusza kamerę w lewo.

      Szybkość poruszania uzależniona jest od wielkości \link MovementSpeed \endlink.
   */
   void MoveLeft();
   /*!
      \brief Porusza kamerę w prawo.

      Szybkość poruszania uzależniona jest od wielkości \link MovementSpeed \endlink.
   */
   void MoveRight();
   /*!
      \brief Włącza lub wyłącza tryb wolnej kamery.
   */
   void TurnFreeCamera();
   /*!
      \brief Ustawia kamerę w pozycji domyślnej.

      Domyślna pozycja to: 0.0f, 1.0f, 0.0f.
   */
   void SetPositionDefault();
   /*!
      \brief Zwraca aktualną pozycję kamery.
   */
   vec3 ReturnPosition() const;
   /*!
      \brief Wyświetla informacje o kamerze.
   */
   void Log() const;
private:
   /*!
      \brief Aktualizacja macierzy projekcji.
   */
   void UpdateProjectionMatrix();
   /*!
      \brief Pozycja kamery.
   */
   vec3 Position = vec3( 0.0f, 1.0f, 0.0f );
   /*!
      \brief Cel kamery, w jaki punkt patrzy.
   */
   vec3 ViewDirection = vec3( 0.0f, 0.0f, -1.0f );
   /*!
      \brief Wektor kierunku.
   */
   vec3 Up = vec3( 0.0f, 1.0f, 0.0f );
   /*!
      \brief Poprzednia pozycja kamery.
   */
   vec3 PreviousPosition = vec3( 0.0f, 1.0f, 0.0f );
   /*!
      \brief Maksymalna pozycja kamery.
   */
   vec3 PositionMax = vec3( 50.0f, 25.0f, 50.0f );
   /*!
      \brief Minimalna pozycja kamery.
   */
   vec3 PositionMin = vec3( -50.0f, -25.0f, -50.0f );
   //Rotation:
   /*!
      \brief Szybkość obrotu kamery.
   */
   vec1 RotationSpeed = vec1( 0.0005f );
   /*!
      \brief Tymczasowy dla obrotu kamery.
   */
   vec3 RotationAround;
   /*!
      \brief Tymczasowy dla obrotu kamery.
   */
   mat4 Rotation;
   //Movement:
   /*!
      \brief Szybkość poruszania się kamery.
   */
   vec1 MovementSpeed = vec1( 0.1f );
   /*!
      \brief Tymczasowy dla poruszania się kamery.
   */
   vec3 MovementDirection;
   //ProjectionMatrix:
   /*!
      \brief Macierz projekcji.
   */
   mat4 ProjectionMatrix;
   /*!
      \brief Kąt widzenia dla kamery, wyrażony w stopniach.
   */
   vec1 VOF = vec1( 45.0f );
   /*!
      \brief Współczynnik szerokości do wysokości, obliczany jako szerokość/wysokość.
   */
   vec1 Aspect = vec1( 4.0f / 3.0f );
   /*!
      \brief Odległość od jakiej rysowane są obiekty.
   */
   vec1 Near = vec1( 0.1 );
   /*!
      \brief Odległość do jakiej rysowane są obiekty.
   */
   vec1 Far = vec1( 100.0f );
   //Free camera:
   /*!
      \brief Wartość logiczna dla włączenie lub wyłączenia wolnej kamery ( \link TurnFreeCamera() \endlink ).
   */
   bool FreeCamera = false;
};

#endif
