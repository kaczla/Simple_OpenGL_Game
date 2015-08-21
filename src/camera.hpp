#ifndef camera_hpp
#define camera_hpp
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

using namespace glm;

class Camera{
public:
   Camera();
   ~Camera();
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
   void Log() const;
private:
   void UpdateProjectionMatrix();
   vec3 Position = vec3( 0.0f, 1.0f, 0.0f );
   vec3 ViewDirection = vec3( 0.0f, 0.0f, -1.0f );
   vec3 Up = vec3( 0.0f, 1.0f, 0.0f );
   //Rotation:
   vec1 RotationSpeed = 0.0005f;
   vec3 RotationAround;
   mat4 Rotation;
   //Movement:
   vec1 MovementSpeed = 0.1f;
   vec3 MovementDirection;
   //ProjectionMatrix:
   mat4 ProjectionMatrix;
   vec1 VOF = 45.0f;
   vec1 Aspect = 4.0f / 3.0f;
   vec1 Near = 0.1;
   vec1 Far = 100.0f;
};

#endif
