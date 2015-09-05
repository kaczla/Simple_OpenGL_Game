#include "camera.hpp"
#include <SDL2/SDL.h>
#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(){
   this->ProjectionMatrix = glm::perspective( glm::radians( this->VOF.x ), this->Aspect.x, this->Near.x, this->Far.x );
}

Camera::Camera( const Camera &camera ){
   this->Position = camera.Position;
   this->ViewDirection = camera.ViewDirection;
   this->Up = camera.Up;

   this->PositionMax = camera.PositionMax;
   this->PositionMin = camera.PositionMin;

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

   this->PositionMax = camera.PositionMax;
   this->PositionMin = camera.PositionMin;

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

void Camera::SetPositionMin( vec3 &min ){
   this->PositionMin = min;
}

void Camera::SetPositionMax( vec3 &max ){
   this->PositionMax = max;
}

void Camera::SetPositionMinMax( vec3 &min, vec3 &max ){
   this->PositionMin = min;
   this->PositionMax = max;
}

void Camera::ChangeVOF( glm::vec1 &in ){
   this->VOF += in;
   if( this->VOF.x < 1.0f ){
      this->VOF.x = 1.0f;
   }
   if( this->VOF.x > 45.0f ){
      this->VOF.x = 45.0f;
   }
   this->UpdateProjectionMatrix();
}

void Camera::UpdateProjectionMatrix(){
   this->ProjectionMatrix = glm::perspective( glm::radians( this->VOF.x ), this->Aspect.x, this->Near.x, this->Far.x );
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
   if( this->FreeCamera ){
      this->PreviousPosition = this->Position;
      this->Position += this->MovementSpeed.x * this->Up;
      if( any( greaterThanEqual( this->Position, this->PositionMax ) ) or
          any( lessThanEqual( this->Position, this->PositionMin ) )
        ){
         this->Position = this->PreviousPosition;
      }
   }
}

void Camera::MoveDown(){
   if( this->FreeCamera ){
      this->PreviousPosition = this->Position;
      this->Position -= this->MovementSpeed.x * this->Up;
      if( any( greaterThanEqual( this->Position, this->PositionMax ) ) or
          any( lessThanEqual( this->Position, this->PositionMin ) )
        ){
         this->Position = this->PreviousPosition;
      }
   }
}

void Camera::MoveForward(){
   this->PreviousPosition = this->Position;
   this->Position.x += this->MovementSpeed.x * this->ViewDirection.x;
   this->Position.z += this->MovementSpeed.x * this->ViewDirection.z;
   if( this->FreeCamera ){
      this->Position.y += this->MovementSpeed.x * this->ViewDirection.y;
   }
   if( any( greaterThanEqual( this->Position, this->PositionMax ) ) or
       any( lessThanEqual( this->Position, this->PositionMin ) )
     ){
      this->Position = this->PreviousPosition;
   }
}

void Camera::MoveBackward(){
   this->PreviousPosition = this->Position;
   this->Position.x -= this->MovementSpeed.x * this->ViewDirection.x;
   this->Position.z -= this->MovementSpeed.x * this->ViewDirection.z;
   if( this->FreeCamera ){
      this->Position.y -= this->MovementSpeed.x * this->ViewDirection.y;
   }
   if( any( greaterThanEqual( this->Position, this->PositionMax ) ) or
       any( lessThanEqual( this->Position, this->PositionMin ) )
     ){
      this->Position = this->PreviousPosition;
   }
}

void Camera::MoveLeft(){
   this->PreviousPosition = this->Position;
   this->MovementDirection = cross( this->ViewDirection, this->Up );//normalize( cross( this->ViewDirection, this->Up ) );
   this->Position -= this->MovementSpeed.x * this->MovementDirection;
   if( any( greaterThanEqual( this->Position, this->PositionMax ) ) or
       any( lessThanEqual( this->Position, this->PositionMin ) )
     ){
      this->Position = this->PreviousPosition;
   }
}

//normalize( ) or fastNormalize( )

void Camera::MoveRight(){
   this->PreviousPosition = this->Position;
   this->MovementDirection = cross( this->ViewDirection, this->Up );//normalize( cross( this->ViewDirection, this->Up ) );
   this->Position += this->MovementSpeed.x * this->MovementDirection;
   if( any( greaterThanEqual( this->Position, this->PositionMax ) ) or
       any( lessThanEqual( this->Position, this->PositionMin ) )
     ){
      this->Position = this->PreviousPosition;
   }
}

void Camera::TurnFreeCamera(){
   if( this->FreeCamera ){
      SDL_Log( "FreeCamera disable\n" );
      this->FreeCamera = false;
   }
   else{
      SDL_Log( "FreeCamera enable\n" );
      this->FreeCamera = true;
   }
}

void Camera::SetPositionDefault(){
   SDL_Log( "Set position default\n" );
   this->Position = vec3( 0.0f, 1.0f, 0.0f );
   this->ViewDirection = vec3( 0.0f, 0.0f, -1.0f );
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
   SDL_Log( "\r     \tPositionMin: %f %f %f\n\tPositionMax: %f %f %f\n",
            this->PositionMin.x, this->PositionMin.y, this->PositionMin.z,
            this->PositionMax.x, this->PositionMax.y, this->PositionMax.z
   );
}
