#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;
layout (location = 2) in vec3 vNormal;

// --------------------- OUT ---------------------
out vec2 texUV;
out vec3 fragPos;
out vec3 normal;

// ------------------- UNIFORM -------------------
uniform mat4 model;
uniform mat4 projview;

uniform sampler2D uWorleyNoise;
uniform float uWaveIncrement;

void main()
{
	texUV = vTex;
	fragPos = vec3(model * vec4(vPos, 1.0f));
	normal = vNormal;

	gl_Position = projview * model * vec4(vPos, 1.0f);
}