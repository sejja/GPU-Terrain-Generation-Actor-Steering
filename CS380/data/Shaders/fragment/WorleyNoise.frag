#version 400 core

#define MAX_POINT_NUM 255

// --------------------- IN ---------------------
in vec2 texUV;

// --------------------- OUT --------------------
out vec3 FragColor;

// ------------------- UNIFORM -------------------
uniform vec3 uPoints[MAX_POINT_NUM];
uniform int uNoiseScale;
uniform float uNoiseDepth;

float map(float val, float oldMin, float oldMax, float newMin, float newMax)
{
	float normalized = (val - oldMin) / (oldMax - oldMin);
	float result = normalized * (newMax - newMin) + newMin;
	return result;
}

void main()
{
	vec2 pixelPos = texUV;

	vec3 color = vec3(0.0);

	float m_dist = 1.0f;
	for (int i = 0; i < MAX_POINT_NUM; i++)
	{
		vec3 point = vec3(pixelPos, uNoiseDepth / 400.f);
		float dist = distance(uPoints[i] / 400.f, point);
		m_dist = min(m_dist, dist);
	}

	float noise = map(m_dist, 0, 75, 0, 255);

	FragColor = vec3(1,1,1);
}