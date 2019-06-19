#version 330 core
out vec4 FragColor;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	

		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
			light.quadratic *(distance * distance));

		//ambient
		//环境光颜色基本等于漫反射颜色
		vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

		vec3 normal = normalize(Normal);

		//specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
		vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular;

		//diffuse
		float diff = max(dot(normal, lightDir), 0);
		vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

		diffuse *= intensity;
		specular *= intensity;

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular;
	
		
		FragColor = vec4(result, 1.0);
}	