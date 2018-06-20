#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    WorldPos = (modelMatrix * position).xyz;
	Normal = mat3(modelMatrix) * normal;
    TexCoords = texCoord;
    
    gl_Position = projectionMatrix * viewMatrix * vec4(WorldPos, 1.0);
}

