#version 330 core

const int MAX_LIGHTS = 8;
const float PI = 3.14159265359;


in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// camera position
uniform vec3 camPos;

// material properties
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights and light properties
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];

uniform int numLights;


float distribution(vec3 N, vec3 H, float rough);
float schlickGGX(vec3 N, vec3 V, float rough);
float geometry(vec3 N, vec3 V, vec3 L, float rough);
vec3 fresnel(vec3 N, vec3 V, vec3 F0);


void main()
{
	vec3 N = normalize(Normal);					// normalize surface Normal
	vec3 V = normalize(camPos - WorldPos);		// and calculate view vector
	
	vec3 F0 = vec3(0.4);
	F0 = mix(F0, albedo, metallic);
	
	vec3 LOut = vec3(0.0);						// computed out light
	for(int i = 0; i < numLights; i++)
	{
		vec3 L = normalize(lightPositions[i] - WorldPos);	// calculate light vector
		vec3 H = normalize(V + L);							// calculate half way vector between light and view
		
		float LDist = length(lightPositions[i] - WorldPos);	// calculate distance of light from object ...
		float attenuation = 1.0 / (LDist * LDist);			// ... and how much of its intensity is lost ...
		vec3 radiance = lightColors[i] * attenuation;		// ... and calculate radiance from that
		
		// compute parts of cook-torrance equation
		vec3 F = fresnel(N, V, F0);
		float D = distribution(N, H, roughness);
		float G = geometry(N, V, L, roughness);
		
		// calculate cook-torrance
		vec3 num = D * G * F;
		float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 spec = num / max(denom, 0.0001);
		
		vec3 specPortion = F;
		vec3 diffPortion = vec3(1.0) - specPortion;
		diffPortion *= 1.0 - metallic;
		
		// calculate contribution of current light
		float NdotL = max(dot(N, L), 0.0);
		LOut += (diffPortion * albedo / PI + spec) * radiance * NdotL;
		
	}

	// add temporary ambient term
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + LOut;
	
	// do HDR tonemapping and gamma correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
} 


// distribution function for cook-torrance brdf
// using Trowbridge-Reitz GGX
float distribution(vec3 N, vec3 H, float rough) 
{
	float a = rough * rough;
	float aSq = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotHSq = NdotH * NdotH;
	
	float denom = NdotHSq * (aSq - 1.0) + 1.0;
	denom = PI * denom * denom;
	
	return aSq / denom;
}

// geometry function for cook-torrance brdf
// using Smiths method (combination of two Schlick-GGX approximations)
float schlickGGX(vec3 N, vec3 V, float rough)
{
	float r = (rough + 1.0);
	float k = (r*r) / 8.0;

	float NdotV = max(dot(N, V), 0.0);
	float denom = NdotV * (1.0 - k) + k;
	
	return NdotV / denom;
}

float geometry(vec3 N, vec3 V, vec3 L, float rough) 
{
	return schlickGGX(N, V, rough) * schlickGGX(N, L, rough);
}

// fesnel function for cook-torrance brdf
// using Schlick approximation
vec3 fresnel(vec3 N, vec3 V, vec3 F0)
{
	float NdotV = max(dot(N, V), 0.0);
	return F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);
}