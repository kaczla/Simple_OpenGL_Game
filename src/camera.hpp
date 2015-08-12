#ifndef camera_hpp
#define camera_hpp
#include <glm/glm.hpp>

using namespace glm;

class Camera{
public:
   Camera();
   ~Camera();
   mat4 getModelToViewMatrix() const;
   void MouseUpdate( const vec2 &Mouse );
   void MoveUp();
   void MoveDown();
   void MoveForward();
   void MoveBackward();
   void MoveLeft();
   void MoveRight();
private:
   vec3 Position = vec3( 1.0f, 1.0f, 1.0f );
   vec3 ViewDirection = vec3( 0.0f, 0.0f, 0.0f );
   vec3 Up = vec3( 0.0f, 1.0f, 0.0f );
   //Rotation:
   float RotationSpeed = 1.0f;
   vec3 RotationAround;
   mat4 Rotation;
   //Movement:
   float MovementSpeed = 0.5f;
   vec3 MovementDirection;
};

#endif
