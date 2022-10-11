#version 400 core

#define POINT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2

#define LIGHT_NUM_MAX 7

struct Light {

    int type;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 position;

	float spot_falloff;
	float spot_innerAngle;
	float spot_outerAngle;

	float const_Att;
	float linear_Att;
	float quadratic_Att;
};

// --------------------- IN ---------------------
in vec2 texUV;
in vec3 normal;
in vec3 fragPos;

// --------------------- OUT --------------------
out vec4 FragColor;

// ------------------- UNIFORM ------------------
uniform Light uLight[LIGHT_NUM_MAX];
uniform int uLightNum;

uniform int uDiffuseMapCount;

uniform float uShinyness;

uniform vec3 uViewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
	for (int i = 0; i < uLightNum; i++)
	{
		// ambient
		vec3 ambient = texture(texture_diffuse1, texUV).rgb;
  		
		// diffuse 
		vec3 norm = normalize(normal);
		vec3 lightDir = normalize(uLight[i].position - fragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = uLight[i].diffuse * diff * texture(texture_diffuse1, texUV).rgb;  
		
		// specular
		vec3 viewDir = normalize(uViewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShinyness);
		vec3 specular = uLight[i].specular * spec * texture(texture_specular1, texUV).rgb;  
		
		// attenuation
		float dist        = length(uLight[i].position - fragPos);
		float attenuation = 1.0f / (uLight[i].const_Att + uLight[i].linear_Att * dist + uLight[i].quadratic_Att * (dist * dist));    

		ambient  *= attenuation;  
		diffuse  *= attenuation;
		specular *= attenuation;   
		    
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
		return;
	}
}