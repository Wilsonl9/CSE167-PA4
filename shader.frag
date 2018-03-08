#version 330 core
// This is a sample fragment shader.

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
}; 

#define NR_POINT_LIGHTS 4

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in float sampleExtraOutput;
in vec3 NormalColor;
in vec3 FragPos;
in vec3 Normal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights;
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform int phong;
uniform int pointCount;
uniform int spotCount;
uniform int dirCount;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.

    //color = vec4(NormalColor, 1.0f);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	if(phong == 1)
	{
		/*//ambient
		vec3 ambient = material.ambient * lightColor;

		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = lightColor * (diff * material.diffuse);

		//specular
		float specularStrength = 0.5f;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = specularStrength * (spec * material.specular) * lightColor;

		result = (ambient + diffuse + specular);*/

	    // Properties
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);
    
	    // == ======================================
	    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
	    // For each phase, a calculate function is defined that calculates the corresponding color
	    // per lamp. In the main() function we take all the calculated colors and sum them up for
		// this fragment's final color.
		// == ======================================
		// Phase 1: Directional lighting
		if(dirCount == 1)
		{
			result += CalcDirLight(dirLight, norm, viewDir);
		}
		// Phase 2: Point lights
		//for(int i = 0; i < pointCount; i++)
	        //result += CalcPointLight(pointLights, norm, FragPos, viewDir);    
		if(pointCount == 1)
		{
			result += CalcPointLight(pointLights, norm, FragPos, viewDir);
		}
	    // Phase 3: Spot light
		if(spotCount == 1)
		{
			result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
		}

	}
	else
	{
		result = NormalColor;
	}
	color = vec4(result, 1.0);


    
/*    color = vec4(result, sampleExtraOutput);*/
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    //float attenuation = 1.0f / (light.constant + light.linear * distance);    
	float attenuation = 1.0f;// / (light.linear * distance);    
    // Combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    
    // Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction)); 
    
    if(theta > light.cutOff) // Remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // Ambient
        vec3 ambient = light.ambient * material.diffuse;
        
        // Diffuse 
        vec3 norm = normalize(Normal);        
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * material.diffuse;
        
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * material.specular;
        
        // Attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation;  // Also remove attenuation from ambient, because if we move too far, the light in spotlight would then be darker than outside (since outside spotlight we have ambient lighting).
        diffuse  *= attenuation;
        specular *= attenuation;   
                
        return ambient + diffuse + specular;
    }
    else    // else, use ambient light so scene isn't completely dark outside the spotlight.
        return light.ambient * material.diffuse;
}
