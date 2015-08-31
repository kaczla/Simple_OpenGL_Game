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

class Camera{
public:
   Camera();
   Camera( const Camera &camera );
   ~Camera();
   Camera & operator=( const Camera &camera );
   mat4 getViewMatrix() const;
   mat4 getProjectionMatrix() const;
   void SetVOF( vec1 &vof );
   void SetAspect( vec1 &aspect );
   void SetNear( vec1 &near );
   void SetFar( vec1 &far );
   void MouseUpdate( const vec2 &Mouse );
   void MoveUp();
   void MoveDown();
   void MoveForward();
   void MoveBackward();
   void MoveLeft();
   void MoveRight();
   void TurnFreeCamera();
   void SetPositionDefault();
   vec3 ReturnPosition() const;
   void Log() const;
private:
   void UpdateProjectionMatrix();
   vec3 Position = vec3( 0.0f, 1.0f, 0.0f );
   vec3 ViewDirection = vec3( 0.0f, 0.0f, -1.0f );
   vec3 Up = vec3( 0.0f, 1.0f, 0.0f );
   vec3 PreviousPosition = vec3( 0.0f, 1.0f, 0.0f );
   //Rotation:
   vec1 RotationSpeed = vec1( 0.0005f );
   vec3 RotationAround;
   mat4 Rotation;
   //Movement:
   vec1 MovementSpeed = vec1( 0.1f );
   vec3 MovementDirection;
   //ProjectionMatrix:
   mat4 ProjectionMatrix;
   vec1 VOF = vec1( 45.0f );
   vec1 Aspect = vec1( 4.0f / 3.0f );
   vec1 Near = vec1( 0.1 );
   vec1 Far = vec1( 100.0f );
   //Free camera:
   bool FreeCamera = false;
};

#endif
