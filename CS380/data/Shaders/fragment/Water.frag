#version 400 core

#define MAX_POINT_NUM 255

// --------------------- IN ---------------------
in vec2 texUV;
in vec3 fragPos;
in vec3 normal;

// --------------------- OUT --------------------
out vec3 FragColor;

// ------------------- UNIFORM -------------------
uniform vec3 uBaseColor;
uniform vec3 uWaveColor;

uniform samplerCube uEnvMap;
uniform sampler2D uWorleyNoise;

uniform vec3 uViewPos;

void main()
{
	//float ratio = 1.0f / refracIndex; 
	// calculate the incident vector
	vec3 incidentVec = normalize(fragPos - uViewPos);
	// set Reflected / Refracted vector
    vec3 refVec = /*(eRenderMode == E_REFRACTION) ? refract(incidentVec, normalize(normal), ratio) :*/ reflect(incidentVec, normalize(normal));

	vec3 reflectionCol = texture(uEnvMap, refVec).rgb;
	vec3 waterCol = uBaseColor + uWaveColor * texture(uWorleyNoise, texUV).r;

	FragColor = vec3(1,1,1);
}