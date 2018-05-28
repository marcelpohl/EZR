#version 330 core
const float PI 3.1415926535897932384626433832795;

out vec4 FragColor;

in DATA
{
	vec4 position;
	vec3 normal;
	vec2 uv;
	vec3 binormal;
	vec3 tangent;
	vec3 cameraPos;
} fs_in;

void main()
{
	FragColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
}