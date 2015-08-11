#ifndef camera_hpp
#define camera_hpp
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;

class Camera{
public:
   Camera();
   ~Camera();
private:
   vec3 VectorEye = vec3( 1.0f, 1.0f, 1.0f );
   vec3 VectorCenter = vec3( 0.0f, 0.0f, 0.0f );
   vec3 VectorUp = vec3( 0.0f, 1.0f, 0.0f );
};

#endif
