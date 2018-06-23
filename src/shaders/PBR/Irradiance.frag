#version 330 core

layout(location = 0) out vec4 FragColor;
in vec3 passTexCoord;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
	// The world vector acts as the normal of a tangent surface
    // from the origin, aligned to WorldPos. Given this normal, calculate all
    // incoming radiance of the environment. The result of this radiance
    // is the radiance of light coming from -Normal direction, which is what
    // we use in the PBR shader to sample irradiance.
    vec3 N = normalize(passTexCoord);

    vec3 irradiance = vec3(0.0);   
    
    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up            = normalize(cross(N, right)); // Das zwiete Normalisiern ist eingentlich nicht notwendig da N und right im rechtenwinkel zueinader sind aber sicher ist sicher.
       
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples)); // PI siehe: http://www.codinglabs.net/article_physically_based_rendering.aspx warum wird der Interne Schätzwert mit 2 multiplizier? Damit es sich leichter kürzen lässt... Nenner und Zähler x2 ändert den Wert nicht
    
    FragColor = vec4(irradiance, 1.0);
}