#version 460 core
#pragma optimization(on)

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_NUM_MAX 7

struct Light {

	int type;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;

	float spot_falloff;
	float spot_innerAngle;
	float spot_outerAngle;

	float const_Att;
	float linear_Att;
	float quadratic_Att;
};

// --------------------- IN ---------------------
in OUT_IN_VARIABLES{
	vec2 texUV;
	flat int texIdx;
	vec3 modelPos;
	vec3 fragPos;
	vec3 normal;
    vec4 surfacePos_light;
} inVar;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM ------------------
uniform Light uLight[LIGHT_NUM_MAX];
uniform int uLightNum;

uniform samplerCube uSkybox_0;
uniform samplerCube uSkybox_1;
uniform samplerCube uSkybox_2;
uniform samplerCube uSkybox_3;

uniform vec3 uViewPos;


uniform sampler2D uShadowMap;


float getPCFShadow(vec4 light_SurfacePos)
{
	// Perspective division
	/*vec3 perspSurfacePos_L = light_SurfacePos.xyz / light_SurfacePos.w;
	
    perspSurfacePos_L = perspSurfacePos_L * 0.5f + 0.5f;

	vec2 texelOffset = 1.0 / textureSize(uShadowMap, 0);
	float newBias = 1.0f / light_SurfacePos.w;
	int neighbor = 20 / 2;

	float sampleCount = 0.0f;
	float accumulatedVisibility = 0;
	for(int x = -neighbor; x <= neighbor; x++)
	{
		for(int y = -neighbor; y <= neighbor; y++)
		{
		
            float pcfDepth = texture(uShadowMap, perspSurfacePos_L.xy + vec2(x, y) * texelOffset).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;      
			/*float D = texture(uShadowMap, perspSurfacePos_L.xy + texelOffset * vec2(x, y)).r;

			// Evaluate visibility for this sample
			accumulatedVisibility += (perspSurfacePos_L.z - newBias > D) ? 1.0f : 0.0f;
			sampleCount++;
		}
	}

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(perspSurfacePos_L.z > 1.0)
        accumulatedVisibility = 0.0;

	return accumulatedVisibility / sampleCount;*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// perform perspective divide
    vec3 projCoords = light_SurfacePos.xyz / light_SurfacePos.w;
    // transform to [0,1] range
    projCoords = vec3(projCoords.x * 0.5 + 0.5, projCoords.y * 0.5 + 0.5, projCoords.z * 0.5 + 0.5);
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    /*vec3 normal = normalize(inVar.normal);
    vec3 lightDir = normalize(inVar.surfacePos_light - inVar.tan_surfacePos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);*/
	
	float bias = 1.0;
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
	
	int neighbour = 20 / 2;
	for(int x = -neighbour; x <= neighbour; x++)
	{
		for(int y = -neighbour; y <= neighbour; y++)
		{
            vec3 pcfDepth = texture(uShadowMap, vec2(projCoords.xy + vec2(x * texelSize.x, y * texelSize.y))).rgb; 
            shadow += pcfDepth.r;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{

	vec3 diffuseTexel;
	switch (inVar.texIdx)
	{
	case 0:
		diffuseTexel = texture(uSkybox_0, inVar.modelPos).rgb;
		break;
	case 1:
		diffuseTexel = texture(uSkybox_1, inVar.modelPos).rgb;
		break;
	case 2:
		diffuseTexel = texture(uSkybox_2, inVar.modelPos).rgb;
		break;
	case 3:
		diffuseTexel = texture(uSkybox_3, inVar.modelPos).rgb;
		break;
	}

	vec3 finalColor = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < uLightNum; i++)
	{
  		
		/*vec3 viewDir = normalize(uViewPos - inVar.fragPos);
		vec3 reflectDir = reflect(-lightDir, normalTexel);  
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normalTexel, halfwayDir), 0.0f), 32.0f); // @TODO: Shinyness
		vec3 specular = uLight[i].specular * spec * specularTexel;  */
		
		// @TODO: Light casting types  
			
		float pcfShadow = 1.0f /*- getPCFShadow(inVar.surfacePos_light)*/;
		finalColor += (0.1 + pcfShadow) * uLight[i].ambient.rgb * diffuseTexel + 
			pcfShadow * uLight[i].diffuse.rgb * max(dot(normalize(inVar.normal), normalize(uLight[i].position - inVar.fragPos)), 0.0f) * diffuseTexel;
	}
	
	FragColor = vec4(finalColor, 1.0f);
}