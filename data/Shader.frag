#version 330 core

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D Texture;
uniform sampler2D Texture_specular;
uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 Specular;
uniform float Shininess;

uniform vec3 Light_position;
uniform vec3 ViewPos;

void main()
{
   vec3 ambient = Ambient * vec3( texture( Texture, UV ) );

   vec3 norm = normalize( Normal );
   vec3 lightDir = normalize( Light_position - FragPos );
   float diff = max( dot( norm, lightDir ), 0.0 );
   vec3 diffuse = Diffuse * diff * vec3( texture( Texture, UV ) );

   vec3 viewDir = normalize( ViewPos - FragPos );
   vec3 reflectDir = reflect( -lightDir, norm );
   float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), Shininess );
   vec3 specular = Specular * spec * vec3( texture( Texture_specular, UV ) );

   color = vec4( ambient + diffuse + specular, 1.0f );
}
