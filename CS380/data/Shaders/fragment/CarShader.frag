#version 460 core

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
in OUT_IN_VARIABLES {
	vec2 texUV;
	vec3 fragPos;
	vec3 tan_lightPos[LIGHT_NUM_MAX];
	vec3 tan_viewPos;
	vec3 tan_surfacePos;
} inVar;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM ------------------
uniform Light uLight[LIGHT_NUM_MAX];
uniform int uLightNum;
uniform int uDiffuseMapCount;
uniform float uShinyness;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D uWaterTex;
uniform vec4 uColor;

void main()
{
	vec4 diffuseTexel = texture(uWaterTex, inVar.texUV);
	vec4 specularTexel = texture(texture_specular1, inVar.texUV);
	vec3 normalTexel = texture(texture_normal1, inVar.texUV).rgb;
	normalTexel = normalize(normalTexel * 2.0f - 1.0f);

	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int i = 0; i < uLightNum; i++)
	{
		// ambient
		vec4 ambient = uLight[i].ambient * diffuseTexel;
  		
		// diffuse 
		vec3 lightDir = normalize(inVar.tan_lightPos[i] - inVar.fragPos);
		float diff = max(dot(lightDir, normalTexel), 0.0f);
		vec4 diffuse = uLight[i].diffuse * diff * diffuseTexel;  
		
		// specular
		vec3 viewDir = normalize(inVar.tan_viewPos - inVar.fragPos);
		vec3 reflectDir = reflect(-lightDir, normalTexel);  
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normalTexel, halfwayDir), 0.0f), 32.0f); // @TODO: Shinyness
		vec4 specular = uLight[i].specular * spec * specularTexel;  
		
		// @TODO: Light casting types  
		    
		finalColor += ambient + diffuse + specular;
	}
	
	FragColor = finalColor * uColor;
}