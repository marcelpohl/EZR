#version 330 core
const int MAX_LIGHTS = 8;
const float PI = 3.1415926535897932384626433832795;
const float GAMMA = 2.2;


out vec4 FragColor;

in DATA
{
	vec3 position;
	vec3 normal;
	vec2 uv;
	vec3 binormal;
	vec3 tangent;
	vec3 cameraPos;
	vec4 fragPosLightSpace;
} fs_in;



// Maps for PBR
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_AOMap;

// lights and light properties
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform sampler2D u_shadowMap;

uniform int numLights;

uniform int displayMode;

struct GeometricAttributes
{
	vec3 pos;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
};

GeometricAttributes geo_Attributes;


vec3 GetDiffuse()
{
	return texture(u_DiffuseMap, fs_in.uv).rgb;
}

float GetMetallic()
{
	return texture(u_MetallicMap, fs_in.uv).r;
}

float GetRoughness()
{
	return texture(u_RoughnessMap, fs_in.uv).r;
}

float GetAO()
{
	return texture(u_AOMap, fs_in.uv).r;
}

vec3 GetNormal()
{
	mat3 toWorld = mat3(geo_Attributes.binormal, geo_Attributes.tangent, geo_Attributes.normal);
	vec3 normal = texture(u_NormalMap, fs_in.uv).rgb * 2.0 - 1.0;
	normal = normalize(toWorld * normal.rgb);
	return normal;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
	return GeometrySchlickGGX(NdotL, roughness) * GeometrySchlickGGX(NdotV, roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float ShadowCalculation()
{
	float shadow = 0.0;
	vec3 shadowCoord = fs_in.fragPosLightSpace.xyz / fs_in.fragPosLightSpace.w;
	
	// sample shadow map in area around current position
	vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
	for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(u_shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).z;
            shadow +=  shadowCoord.z - 0.00005 > pcfDepth ? 1.0 : 0.0;        
			
        }    
    }
    shadow /= 9.0;
	
	if (shadowCoord.z > 1.0)
		shadow = 0.0;
	
	return 1.0 - shadow;

}



void main()
{
	geo_Attributes.pos = fs_in.position.xyz;
	geo_Attributes.normal = normalize(fs_in.normal);
	geo_Attributes.binormal = normalize(fs_in.binormal);
	geo_Attributes.tangent = normalize(fs_in.tangent);

	switch(displayMode)
	{
	case 0:
		break;
	case 1:
		FragColor = vec4(GetDiffuse(), 1.0f);
		return;
	case 2:
		FragColor = vec4(GetNormal(), 1.0f);
		return;
	case 3:
		float metal = GetMetallic();
		FragColor = vec4(metal, metal, metal, 1.0f);
		return;
	case 4:
		float rough = GetRoughness();
		FragColor = vec4(rough, rough, rough, 1.0f);
		return;
	case 5:
		float ao = GetAO();
		FragColor = vec4(ao, ao, ao, 1.0f);
		return;
	}
	
	vec3  diffuse   = GetDiffuse();
	float metallic  = GetMetallic();
	float roughness = GetRoughness();
	float ao        = GetAO();
	roughness 		= roughness*roughness;	// TODO tweak looks (this line is optional)
	
	vec3 normal  = GetNormal();
	vec3 viewVec = normalize(fs_in.cameraPos - fs_in.position);
	
	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the diffuse color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, diffuse, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numLights; ++i) 
    {
        // calculate per-light radiance
        vec3 lightVec = normalize(lightPositions[i] - fs_in.position);
        vec3 halfVec  = normalize(viewVec + lightVec);
        float distance = length(lightPositions[i] - fs_in.position);
        float attenuation = 1.0 / distance;// (distance * distance); 	// TODO tweak looks
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, halfVec, roughness);   
        float G   = GeometrySmith(normal, viewVec, lightVec, roughness);      
        vec3 F    = fresnelSchlick(max(dot(halfVec, viewVec), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(normal, viewVec), 0.0) * max(dot(normal, lightVec), 0.0) + 0.001;
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(normal, lightVec), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * diffuse / PI + specular) * radiance * NdotL;
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * diffuse * ao;
	
	// shadows
	float shadow = ShadowCalculation();
    
    vec3 color = ambient + shadow * Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/GAMMA)); 

    FragColor = vec4(color, 1.0);
	
}