#version 400 core

// --------------------- IN ---------------------
in vec2 texUV;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM ------------------
uniform sampler2D uTextureData;

void main()
{
	FragColor = texture(uTextureData, texUV);
}