// 材质的shader定义
#version 330 core

struct Material 
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;    
    float Shininess;
}; 