#version 400 core

layout (location = 0) in vec3 vPos;

// ------------------- UNIFORM -------------------
uniform mat4 model;
uniform mat4 projview;

// --------------------- OUT ---------------------
out vec3 texUV;

void main()
{
	texUV = 2.0f * vPos;
	vec4 tPos = projview * model * vec4(vPos.x, vPos.y, vPos.z, 1.0);
	gl_Position = tPos.xyww;
}