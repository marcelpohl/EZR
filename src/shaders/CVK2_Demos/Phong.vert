#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 passPosition;
out vec3 passNormal;
out vec2 passTexCoord;

void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

  // Transform the position correctly into view space
  // and pass it to the fragment shader
  passPosition = (viewMatrix * modelMatrix * position).xyz;

  // Transform the normal correctly into view space
  // and pass it to the fragment shader
  mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
  passNormal = normalize(normalMatrix * normal);

  // Texture coords don't need to be transformed
  passTexCoord = texCoord;
}
