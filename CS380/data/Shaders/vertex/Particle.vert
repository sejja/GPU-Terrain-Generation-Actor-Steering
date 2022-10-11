#version 440 core

// --------------------- IN ---------------------
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;
layout(location = 5) in vec4 vColor;
layout(location = 6) in mat4 vModel;

// --------------------- OUT ---------------------
out OUT_IN_VARIABLES {
	vec2 texUV;
	vec4 partCol;
} outVar;

// ------------------- UNIFORM -------------------
uniform mat4 projview;


uniform vec3 uViewPos;

void main()
{
    outVar.texUV  = vUV;
	outVar.partCol = vColor;
    
    gl_Position = vec4(vPos, 1.0f);
}