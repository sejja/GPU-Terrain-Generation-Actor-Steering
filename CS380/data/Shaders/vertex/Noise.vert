#version 460 core
#pragma optimize(On)

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

out OUT_IN_VARIABLES {
	vec2 texUV;
	vec3 pos;
} outVar;

void main()
{
	outVar.pos = vPos;
	outVar.texUV = vTex;
	gl_Position = vec4(vPos, 1.0f);
}