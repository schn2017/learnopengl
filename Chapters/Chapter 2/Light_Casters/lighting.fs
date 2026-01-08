#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    // Both for spotlights
    vec3 position; // For positional lights
    vec3 direction; // For directional lights
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Attenuation
    float constant;
    float linear;
    float quadratic;

    // Spotlight cutoff
    float cutOff;
    float outerCutOff;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;
  
uniform Material material;
uniform Light light;  

uniform vec3 viewPos;

void main()
{   
    //vec3 lightDir = normalize(-light.direction); directinonal

    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    ambient *= attenuation; 

    // diffuse 
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    diffuse *= attenuation;
    diffuse *= intensity;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    specular *= attenuation;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}