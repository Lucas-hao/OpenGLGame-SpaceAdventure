#version 430 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos[2];
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct PointLight {
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 viewPos;
uniform PointLight pointLights[2];

vec3 CalcPointLight(PointLight light, vec3 lightPos ,vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec3 resultColor = CalcPointLight(pointLights[0], fs_in.TangentLightPos[0] , normal, fs_in.TangentFragPos, viewDir);
    resultColor += CalcPointLight(pointLights[1], fs_in.TangentLightPos[1] ,normal, fs_in.TangentFragPos, viewDir);
    FragColor = vec4(resultColor, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    vec3 lightDir = normalize(lightPos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = min(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 1.0f);    
    // combine results
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color * light.color;
    vec3 specular = light.specular * spec * color * light.color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
