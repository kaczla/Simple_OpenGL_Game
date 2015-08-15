#include "camera.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(){
   this->ProjectionMatrix = glm::perspective( this->VOF.x, this->Aspect.x, this->Near.x, this->Far.x );
}

Camera::~Camera(){

}

mat4 Camera::getModelToViewMatrix() const{
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
