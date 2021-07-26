#version 330 core

layout (location = 0) out vec4 FragColor;

//struct Material 
//{
//	sampler2D diffuse;
//	sampler2D specular;
//	float shininess;
//};

//struct Light 
//{
//	vec3 direction;
//
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//};

in vec3 FragPos;
in vec3 Normal;

out vec4 color;
//uniform Material material;
//uniform Light light;

in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//obtain normal from normal map in range 0,1
	vec3 normal = texture(normalMap, TexCoords).rgb;

	//transform normal vector to range -1,1
	//the normal is in tangent space
	normal = normalize(normal * 2.0 - 1.0);

	//get diffuse color
	vec3 color = texture(diffuseMap, TexCoords).rgb;

	//ambient
	vec3 ambient = 0.1 * color;

	//diffuse
	//vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	vec3 lightDir = normalize(-TangentLightPos);
	//float diff = max(dot(normal, lightDir), 0.0);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	//specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}