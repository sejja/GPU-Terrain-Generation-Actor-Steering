#version 400 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoords;

// ------------------- UNIFORM -------------------
uniform mat4 model;
uniform mat4 projview;

// --------------------- OUT ---------------------
out vec2 texUV;

void main()
{
	texUV = vTexCoords;
	gl_Position = vec4(vPos, 1.0f);
}