#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightTransformMatrix;

uniform vec3 u_cameraPosition;

out DATA
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec3 binormal;
	vec3 tangent;
	vec3 cameraPos;
	vec4 fragPosLightSpace;
} vs_out;

void main()
{
	vec4 pos = modelMatrix * position;
	vs_out.position = pos.xyz;
	gl_Position = projectionMatrix * viewMatrix * pos;
	
	mat3 model = mat3(modelMatrix);
	
	vs_out.normal = model * normal;
	vs_out.binormal = model * bitangent;
	vs_out.tangent = model * tangent;
	vs_out.uv = texCoord;
	vs_out.cameraPos = u_cameraPosition;
	vs_out.fragPosLightSpace = lightTransformMatrix * pos;
}