// See https://thebookofshaders.com/11/ for more information

#version 460 core
#pragma optimize(on)

// --------------------- IN ---------------------
in OUT_IN_VARIABLES {
	vec2 texUV;
	vec3 pos;
} inVar;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM -------------------
uniform vec2 uOffset;
uniform vec2 uNoiseSize;

vec2 rand2(vec2 vec){
    vec = vec2( dot(vec,vec2(127.1,311.7)),
                dot(vec,vec2(269.5,183.3)) );
    return -1.0 + 2.0 * fract(sin(vec) * 43758.5453123);
}


float noise(const vec2 vec)
{
    const vec2 i = floor(vec); // integer part
    const vec2 f = fract(vec); // Fractional part
    const vec2 smoothness = f * f * (3.0f - 2.0f * f); // cubic smoothness

    return mix( mix( dot( rand2(i), f),
                     dot( rand2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), smoothness.x),
                mix( dot( rand2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( rand2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), smoothness.x), smoothness.y);
}

void main()
{    
    FragColor = vec4(vec3(noise((inVar.pos.xy + uOffset) * uNoiseSize) * 0.5f + 0.5f), 1.0f);
}  