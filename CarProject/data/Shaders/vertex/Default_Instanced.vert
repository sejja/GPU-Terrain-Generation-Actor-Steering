#version 460 core
#pragma optimization(on)

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in int  vTexIdx;
layout (location = 6) in vec3 vOffset;

#define LIGHT_NUM_MAX 7

struct Light {

    int type;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
    vec3 position;

	float spot_falloff;
	float spot_innerAngle;
	float spot_outerAngle;

	float const_Att;
	float linear_Att;
	float quadratic_Att;
};

// ------------------- UNIFORM -------------------
uniform Light uLight[LIGHT_NUM_MAX];
uniform int uLightNum;

uniform mat4 projview;

// --------------------- OUT ---------------------
out OUT_IN_VARIABLES {
	vec2 texUV;
	flat int texIdx;
	vec3 modelPos;
	vec3 fragPos;
	vec3 normal;
    vec4 surfacePos_light;
} outVar;

mat4 modelMtx(float x, float y, float z, float scale){
    return mat4(
        vec4(scale, 0.0, 0.0, 0.0),
        vec4(0.0, scale, 0.0, 0.0),
        vec4(0.0, 0.0, scale, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

void main()
{
	mat4 model = modelMtx(vOffset.x, vOffset.y, vOffset.z, 1.0 / 6.0);
	
	outVar.texIdx = vTexIdx;
	outVar.texUV = vTexCoord;
	outVar.modelPos = vPos.xyz;
	outVar.fragPos = (model * vec4(vPos, 1.0f)).xyz;
	outVar.normal = mat3(transpose(inverse(model))) * vNormal;
	gl_Position = projview * model * vec4(vPos, 1.0f);
}