#version 400 core

layout (location = 0) in vec3 vPos;

// ------------------- UNIFORM -------------------
uniform mat4 model;
uniform mat4 projview;

// --------------------- OUT ---------------------

void main()
{
	gl_Position = projview * model * vec4(vPos, 1.0f);
}