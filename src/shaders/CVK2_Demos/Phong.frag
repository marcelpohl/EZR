#version 330 core

const int MAX_LIGHTS = 8;

in vec3 passPosition;
in vec3 passNormal;
in vec2 passTexCoord;

out vec4 fragmentColor;

uniform struct {
  float kd, ks, kt;
  vec3 diffColor;
  vec3 specColor;
  float shininess;
} mat;

uniform struct LIGHT {
  vec4 pos;            // check .w: 0 = directional, 1 = point
  vec3 col;
  vec3 spot_direction; 
  float spot_exponent; 
  float spot_cutoff;   // if cutoff == 0, no spotlight
} light[MAX_LIGHTS];

uniform struct FogParams {
  vec3 col;
  float start;   // (only for linear fog)
  float end;     // (only for linear fog)
  float density;
  int mode;      // 1 = linear, 2 = exp, 3 = exp2
} fog;

uniform int numLights;
uniform vec3 lightAmbient;
uniform int useColorTexture;
uniform sampler2D colorTexture;
uniform mat4 viewMatrix;


///
float getFogFactor(FogParams fog, float z) {
  float f = 0.0;
  if (fog.mode == 1) {
    f = (fog.end - z)/(fog.end - fog.start);
  } else if (fog.mode == 2) {
    f = exp(-fog.density * z);
  } else if(fog.mode == 3) {
    f = exp(-pow(fog.density * z, 2.0));
  }

  f = clamp(f, 0.0, 1.0);
  return f;
}

void main() {

  // Diffuse base
  vec4 diffuseColor;
  if (useColorTexture != 0) {
    diffuseColor = texture(colorTexture, passTexCoord).rgba;
  } else {
    diffuseColor = vec4(mat.diffColor, 1.0f - mat.kt);
  }

  // Ambient base
  fragmentColor.rgb = mat.kd * diffuseColor.rgb * lightAmbient;
  fragmentColor.a = diffuseColor.a;

  // Phong lighting for each light
  for (int i = 0; i < numLights; i++) {

    // Diffuse term
    vec3 lightCamCoord = (viewMatrix * light[i].pos).xyz;
    vec3 lightVector;
    if (light[i].pos.w > 0.001f) {
      lightVector = normalize(lightCamCoord - passPosition);
    } else {
      lightVector = normalize(lightCamCoord);
    }
    float cos_phi = max(dot(passNormal, lightVector), 0.0f);

    // Specular term
    vec3 eye = normalize(-passPosition);
    vec3 reflection = normalize(reflect(-lightVector, passNormal));
    float cos_psi_n = pow(max(dot(reflection, eye), 0.0f), mat.shininess);

    // Spotlight
    float spot = 1.0f;
    if (light[i].spot_cutoff >= 0.001f) {
      float cos_phi_spot = max(dot(-lightVector, mat3(viewMatrix) * light[i].spot_direction), 0.0f);
      bool inRange = cos_phi_spot >= cos(light[i].spot_cutoff);
      spot = inRange ? pow(cos_phi_spot, light[i].spot_exponent) : 0.0f;
    }

    // All together
    fragmentColor.rgb += mat.kd * spot * diffuseColor.rgb * cos_phi * light[i].col;
    fragmentColor.rgb += mat.ks * spot * mat.specColor * cos_psi_n * light[i].col;
  }

  // Add fog
  if (fog.mode != 0) {
    float f = getFogFactor(fog, length(passPosition));
    fragmentColor.rgb = f * fragmentColor.rgb + (1.0 - f) * fog.col;
  }
}
