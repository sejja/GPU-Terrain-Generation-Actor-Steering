#version 460 core
#pragma optimization(on)

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in int  vTexIdx;
layout (location = 6) in vec3 vOffset;

// ------------------- UNIFORM -------------------
uniform mat4 projview;

// --------------------- OUT ---------------------

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
	gl_Position = projview * model * vec4(vPos, 1.0f);
}