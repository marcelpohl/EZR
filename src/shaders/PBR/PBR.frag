#version 330 core
const float PI = 3.1415926535897932384626433832795;
const float GAMMA = 2.2;

struct GeometricAttributes
{
	vec3 pos;
	vec3 normal;
	vec3 binormal;
	vec3 tangent;
};



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



// Maps for PBR
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

GeometricAttributes geo_Attributes;

vec4 GammaCorrectTexture(sampler2D tex, vec2 uv)
{
	vec4 samp = texture(tex, uv);
	return vec4(pow(samp.rgb, vec3(GAMMA)), samp.a);
}

vec3 GammaCorrectTextureRGB(sampler2D tex, vec2 uv)
{
	vec4 samp = texture(tex, uv);
	return vec3(pow(samp.rgb, vec3(GAMMA)));
}

vec3 GetNormal()
{
	mat3 toWorld = mat3(geo_Attributes.binormal, geo_Attributes.tangent, geo_Attributes.normal);
	vec3 normal = texture(u_NormalMap, fs_in.uv).rgb * 2.0 - 1.0;
	normal = normalize(toWorld * normal.rgb);
	return normal;
}

void main()
{
	geo_Attributes.pos = fs_in.position.xyz;
	geo_Attributes.normal = normalize(fs_in.normal);
	geo_Attributes.binormal = normalize(fs_in.binormal);
	geo_Attributes.tangent = normalize(fs_in.tangent);

	FragColor = mix(texture(u_NormalMap, fs_in.uv), texture(u_DiffuseMap, fs_in.uv), 0.5);
}