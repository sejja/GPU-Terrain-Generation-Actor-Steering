#version 460 core
#pragma optimize(on)

#define MAX_POINT_NUM 255

// --------------------- IN ---------------------
in vec2 texUV;
in vec3 fragPos;
in vec3 normal;

// --------------------- OUT --------------------
out vec3 FragColor;

// ------------------- UNIFORM -------------------
uniform sampler2D uBlendMap;
uniform sampler2D uTexture_def;
uniform sampler2D uTexture_r;
uniform sampler2D uTexture_g;
uniform sampler2D uTexture_b;

uniform vec2 uGridSize;

void main()
{
	vec4 blendMapTexel = texture(uBlendMap, texUV);
	vec4 _texturecol = texture(uTexture_def, texUV * uGridSize);
	vec4 defTexel = _texturecol * (1 - (blendMapTexel.r + blendMapTexel.g + blendMapTexel.b));
	vec4 redTexel = _texturecol * blendMapTexel.r;
	vec4 greenTexel = _texturecol * blendMapTexel.g;
	vec4 blueTexel = _texturecol * blendMapTexel.b;

	vec4 finalCol = defTexel + redTexel + greenTexel + blueTexel;

	FragColor = vec3(0, 0, 0);
}