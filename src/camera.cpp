#include "camera.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

Camera::Camera(){

}

Camera::~Camera(){
   
}

mat4 Camera::getModelToViewMatrix() const{
   return lookAt( this->Position, this->Position + this->ViewDirection, this->Up );
}

void Camera::MouseUpdate( const vec2 &Mouse ){
   /*
   this->ViewDirection = mat3( rotate( Mouse.x * this->RotationSpeed, this->Up ) ) * this->ViewDirection;
   this->RotationAround = cross( this->ViewDirection, this->Up );
   this->ViewDirection = mat3( rotate( Mouse.y * this->RotationSpeed, this->RotationAround ) ) * this->ViewDirection;
   */
   //faster:
   this->RotationAround = cross( this->ViewDirection, this->Up );
   this->Rotation = rotate( Mouse.x * this->RotationSpeed, this->Up ) * rotate( Mouse.y * this->RotationSpeed, this->RotationAround );
   this->ViewDirection = mat3( this->Rotation ) * this->ViewDirection;
}

void Camera::MoveUp(){
   this->Position += this->MovementSpeed * this->Up;
}

void Camera::MoveDown(){
   this->Position -= this->MovementSpeed * this->Up;
}

void Camera::MoveForward(){
   this->Position += this->MovementSpeed * this->ViewDirection;
}

void Camera::MoveBackward(){
   this->Position -= this->MovementSpeed * this->ViewDirection;
}

void Camera::MoveLeft(){
   this->MovementDirection = cross( this->ViewDirection, this->Up );
   this->Position -= this->MovementSpeed * this->MovementDirection;
}

void Camera::MoveRight(){
   this->MovementDirection = cross( this->ViewDirection, this->Up );
   this->Position += this->MovementSpeed * this->MovementDirection;
}
