#version 460 core
#pragma optimize(on)
#define MAX_POINTS 750

// --------------------- IN ---------------------
in OUT_IN_VARIABLES {
	vec2 texUV;
	vec3 pos;
} inVar;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM -------------------
uniform vec2 uPoints[MAX_POINTS];

void main()
{
    float closestDist = 999999.9f;
    for (int i = 0; i < MAX_POINTS; i++)
    {
        const vec2 dirVec = uPoints[i] - inVar.texUV;
        closestDist = min(closestDist, dot(dirVec, dirVec));
    }

    FragColor = vec4(vec3(pow(60.0 * closestDist, 0.2f)), 1.0f);
}  