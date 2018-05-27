#version 330 core
const int MAX_LIGHTS = 8;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

out vec3 passWorldPos;
out vec3 passNormal;
out vec3 passLightVector[MAX_LIGHTS];
out vec3 passEyeVector;
out vec2 passTexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform int numLightsVert;
uniform vec3 lightPositionsVert[MAX_LIGHTS];

uniform int useTexturesVert;

void main()
{
    passWorldPos = (modelMatrix * position).xyz;
	gl_Position = projectionMatrix * viewMatrix * vec4(passWorldPos, 1.0);
	passNormal = mat3(modelMatrix) * normal;
	passTexCoords = texCoord;
	
	if (useTexturesVert == 1) 
	{
		// Calculate matrix that goes from camera space to tangent space 
		mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
		vec3 normal_cs = normalize((normalMatrix * vec4(normal,0)).xyz);
		vec3 tangent_cs = normalize((normalMatrix * vec4(tangent,0)).xyz);
		vec3 bitangent_cs = cross(tangent_cs, normal_cs);
		mat3 tbn = 	transpose(mat3(
						tangent_cs,
						bitangent_cs,
						normal_cs
					));
		// Calculate position in camera space
		vec3 position_cs = (viewMatrix * modelMatrix * position).xyz;
		// Calculate eye vector (tangent space)
		vec3 eye_cs = normalize(-position_cs);
		passEyeVector = tbn * eye_cs;
		for(int i = 0; i < numLightsVert; ++i) 
		{
			// Calculate light vector (tangent space)
			vec3 lightPosition_cs = (viewMatrix * modelMatrix * vec4(lightPositionsVert[i], 1.0f)).xyz;
			passLightVector[i] = tbn * normalize(lightPosition_cs - position_cs);
		}
	}
}

