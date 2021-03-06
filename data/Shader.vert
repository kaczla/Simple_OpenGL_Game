#version 330 core
layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 uv;
layout ( location = 2 ) in vec3 normal;

out vec2 UV;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4( position, 1.0f );
   //This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
   // UV = vec2( uv.x, 1.0 - uv.y);
   //otherwise use: (or convert before load into shader)
   UV = uv;
   Normal = mat3( transpose( inverse( model ) ) ) * normal;
   FragPos = vec3( model * vec4( position, 1.0f ) );
}
