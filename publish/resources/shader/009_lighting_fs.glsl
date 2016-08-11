#version 330 core

struct Material 
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;    
    float Shininess;
}; 

struct Light
{
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

in vec3 FragPos;  
in vec3 Normal;  
  
out vec4 color;
  
uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = light.Ambient * material.Ambient;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.Position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Diffuse * (diff * material.Diffuse);
    
    // Specular
    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    vec3 specular = light.Specular * (spec * material.Specular);  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
} 