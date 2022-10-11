#version 400 core

// --------------------- IN ---------------------
in vec3 texUV;

// --------------------- OUT --------------------
out vec4 FragColor;

uniform samplerCube uSkybox;

void main()
{
   FragColor = texture(uSkybox, texUV);
}