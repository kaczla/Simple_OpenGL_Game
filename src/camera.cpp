#include "camera.hpp"
#include <SDL2/SDL.h>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(){
   this->ProjectionMatrix = glm::perspective( this->VOF.x, this->Aspect.x, this->Near.x, this->Far.x );
}

Camera::Camera( const Camera &camera ){
   this->Position = camera.Position;
   this->ViewDirection = camera.ViewDirection;
   this->Up = camera.Up;

   this->RotationSpeed = camera.RotationSpeed;
   this->MovementSpeed = camera.MovementSpeed;

   this->ProjectionMatrix = camera.ProjectionMatrix;

   this->VOF = camera.VOF;
   this->Aspect = camera.Aspect;
   this->Near = camera.Near;
   this->Far = camera.Far;
}

Camera::~Camera(){

}

Camera & Camera::operator=( const Camera &camera ){
   this->Position = camera.Position;
   this->ViewDirection = camera.ViewDirection;
   this->Up = camera.Up;

   this->RotationSpeed = camera.RotationSpeed;
   this->MovementSpeed = camera.MovementSpeed;

   this->ProjectionMatrix = camera.ProjectionMatrix;

   this->VOF = camera.VOF;
   this->Aspect = camera.Aspect;
   this->Near = camera.Near;
   this->Far = camera.Far;

   return *this;
}

mat4 Camera::getViewMatrix() const{
   return lookAt( this->Position, this->Position + this->ViewDirection, this->Up );
}

mat4 Camera::getProjectionMatrix() const{
   return this->ProjectionMatrix;
}

void Camera::SetVOF( vec1 &vof ){
   this->VOF = vof;
   this->UpdateProjectionMatrix();
}

void Camera::SetAspect( vec1 &aspect ){
   this->Aspect = aspect;
   this->UpdateProjectionMatrix();
}

void Camera::SetNear( vec1 &near ){
   this->Near = near;
   this->UpdateProjectionMatrix();
}

void Camera::SetFar( vec1 &far ){
   this->Far = far;
   this->UpdateProjectionMatrix();
}

void Camera::UpdateProjectionMatrix(){
   this->ProjectionMatrix = glm::perspective( this->VOF.x, this->Aspect.x, this->Near.x, this->Far.x );
}

void Camera::MouseUpdate( const vec2 &Mouse ){
   /*
   this->ViewDirection = mat3( rotate( Mouse.x * this->RotationSpeed.x, this->Up ) ) * this->ViewDirection;
   this->RotationAround = cross( this->ViewDirection, this->Up );
   this->ViewDirection = mat3( rotate( Mouse.y * this->RotationSpeed.x, this->RotationAround ) ) * this->ViewDirection;
   */
   //faster:
   this->RotationAround = cross( this->ViewDirection, this->Up );
   this->Rotation = rotate( Mouse.x * this->RotationSpeed.x, this->Up ) * rotate( Mouse.y * this->RotationSpeed.x, this->RotationAround );
   this->ViewDirection = mat3( this->Rotation ) * this->ViewDirection;
}

void Camera::MoveUp(){
   this->Position += this->MovementSpeed.x * this->Up;
}

void Camera::MoveDown(){
   this->Position -= this->MovementSpeed.x * this->Up;
}

void Camera::MoveForward(){
   this->Position += this->MovementSpeed.x * this->ViewDirection;
}

void Camera::MoveBackward(){
   this->Position -= this->MovementSpeed.x * this->ViewDirection;
}

void Camera::MoveLeft(){
   this->MovementDirection = cross( this->ViewDirection, this->Up );//normalize( cross( this->ViewDirection, this->Up ) );
   this->Position -= this->MovementSpeed.x * this->MovementDirection;
}

//normalize( ) or fastNormalize( )

void Camera::MoveRight(){
   this->MovementDirection = cross( this->ViewDirection, this->Up );//normalize( cross( this->ViewDirection, this->Up ) );
   this->Position += this->MovementSpeed.x * this->MovementDirection;
}

vec3 Camera::ReturnPosition() const{
   return this->Position;
}

void Camera::Log() const{
   SDL_Log( "Camera:\n\tPosition: %f %f %f\n\tTarger  : %f %f %f\n\tUp      : %f %f %f\n\tVOF   : %f\tAspect: %f\tNear  : %f\tFar   : %f\n\tRotationSpeed: %f\tMovementSpeed: %f\n",
            this->Position.x, this->Position.y, this->Position.z,
            this->ViewDirection.x, this->ViewDirection.y, this->ViewDirection.z,
            this->Up.x, this->Up.y, this->Up.z,
            this->VOF.x, this->Aspect.x, this->Near.x, this->Far.x,
            this->RotationSpeed.x, this->MovementSpeed.x
   );
}
