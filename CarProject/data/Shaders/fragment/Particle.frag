#version 440 core

// --------------------- IN ---------------------
in OUT_IN_VARIABLES {
	vec2 texUV;
	vec4 partCol;
} inVar;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM -------------------
layout(location = 2) uniform sampler2D uBuffer;

void main()
{
	FragColor = vec4(1,1,1,1);
	/*FragColor = ubEmissive ? vec4(0,0,0,0) : texture(uBuffer, inVar.texUV) * inVar.partCol;
	EmissionColor = ubEmissive ? texture(uBuffer, inVar.texUV) * inVar.partCol : vec4(0,0,0,0);
	BrightColor = vec4(0,0,0,0);*/
}