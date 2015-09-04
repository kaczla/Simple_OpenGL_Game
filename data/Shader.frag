#version 330 core

struct Material_{
   sampler2D Texture;
   sampler2D Texture_specular;
   vec3 Ambient;
   vec3 Diffuse;
   vec3 Specular;
   float Shininess;
};

struct Directional_Light{
   vec3 Position;
   vec3 Ambient;
   vec3 Diffuse;
   vec3 Specular;
};

struct Point_Light{
   vec3 Position;
   float Constant;
   float Linear;
   float Quadratic;
};

#define Max_Point_Light 1

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material_ Material;
uniform Directional_Light DirectionalLight;
uniform Point_Light PointLight[Max_Point_Light];
uniform vec3 ViewPos;

vec3 CalculateDirectionalLight( Directional_Light DirectionalLight_, vec3 normal_, vec3 viewDir_, vec3 fragPos_ );
vec3 CalculatePointLight( Point_Light PointLight_, vec3 normal_, vec3 viewDir_, vec3 fragPos_ );

void main()
{
   vec3 normal = normalize( Normal );
   vec3 viewDir = normalize( ViewPos - FragPos );
   vec3 result = vec3( 0.0f );
   result = CalculateDirectionalLight( DirectionalLight, normal, viewDir, FragPos  );
   for( int i = 0; i < Max_Point_Light; ++i ){
      result += CalculatePointLight( PointLight[i], normal, viewDir, FragPos );
   }
   color = vec4( result, 1.0f );
}

vec3 CalculateDirectionalLight( Directional_Light DirectionalLight_, vec3 normal_, vec3 viewDir_, vec3 fragPos_ ){
   vec3 lightDir = normalize( DirectionalLight_.Position - fragPos_ );
   // Diffuse shading
   float diff = max( dot( normal_, lightDir ), 0.0 );
   // Specular shading
   vec3 reflectDir = reflect( -lightDir, normal_ );
   float spec = pow( max( dot( viewDir_, reflectDir ), 0.0 ), Material.Shininess );
   // Combine results
   vec3 ambient = DirectionalLight_.Ambient * Material.Ambient * vec3( texture( Material.Texture, UV ) );
   vec3 diffuse = DirectionalLight_.Diffuse * Material.Diffuse * diff * vec3( texture( Material.Texture, UV ) );
   vec3 specular = DirectionalLight_.Specular * Material.Specular * spec * vec3( texture( Material.Texture_specular, UV ) );
   return ( ambient + diffuse + specular );
}

vec3 CalculatePointLight( Point_Light PointLight_, vec3 normal_, vec3 viewDir_, vec3 fragPos_ ){
   vec3 lightDir = normalize( PointLight_.Position  - fragPos_ );
   // Diffuse shading
   float diff = max( dot( normal_, lightDir ), 0.0 );
   // Specular shading
   vec3 reflectDir = reflect( -lightDir, normal_ );
   float spec = pow( max( dot( viewDir_, reflectDir ), 0.0 ), Material.Shininess );
   // Attenuation
   float distance = length( PointLight_.Position - fragPos_ );
   float attenuation = 1.0 / ( PointLight_.Constant + PointLight_.Linear * distance + PointLight_.Quadratic * ( distance * distance ) );
   // Combine results
   vec3 ambient = Material.Ambient * vec3( texture( Material.Texture, UV ) );
   vec3 diffuse = Material.Diffuse * diff * vec3( texture( Material.Texture, UV ) );
   vec3 specular = Material.Specular * spec * vec3( texture( Material.Texture_specular, UV ) );
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;
   return( ambient + diffuse + specular );
}
