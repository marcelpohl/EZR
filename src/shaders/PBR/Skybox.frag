#version 330 core
out vec4 fragmentColor;

in vec3 passTexCoord;

uniform samplerCube u_skybox;

void main()
{   
    fragmentColor = texture(u_skybox, passTexCoord);
}