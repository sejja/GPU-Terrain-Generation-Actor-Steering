#version 400 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoords;

// --------------------- OUT ---------------------
out vec2 texUV;

void main()
{
	gl_Position = vec4(vPos, 1.0f);
}