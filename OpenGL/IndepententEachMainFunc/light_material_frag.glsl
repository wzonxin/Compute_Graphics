#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
	//ambient
	vec3 ambient = material.ambient * light.ambient;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
	vec3 specular = (material.specular * spec) * light.specular;

	//diffuse
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}	