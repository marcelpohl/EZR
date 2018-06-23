#version 330 core

layout (location = 0) in vec4 position;

out vec3 passTexCoord;

void main() {

  gl_Position = position;

  passTexCoord = position.xyz;
}