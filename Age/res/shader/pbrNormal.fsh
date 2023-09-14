out vec4 FragColor;

in vec2 fragUv;
in vec3 fragPos;
in mat3 TBN;

uniform vec3 cameraPos;

struct DirectionalLight
{
    vec3 dir, ambient, color;
};
struct PointLight
{
	vec3 pos, ambient, color;
    float constant, linear, quadratic;
};
struct SpotLight
{
    vec3 pos, dir, ambient, color;
    float cutOff, outerCutOff;
    float constant, linear, quadratic;
};

vec3 computePointLight(in PointLight light, vec3 dif, vec3 spec, float shininess, vec3 norm)
{
    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float diffFactor = max(dot(lightDir, norm), 0.f);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.f), shininess);
    vec3 ambientCol = dif * light.ambient;
    vec3 diffuse = dif * diffFactor;
    vec3 specular = spec * specFactor;
    vec3 lightColor = (diffuse + specular) * light.color + ambientCol;

    float dist = distance(light.pos, fragPos);
    float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));  

    return lightColor * attenuation;
}

vec3 computeDirectionalLight(in DirectionalLight light, vec3 dif, vec3 spec, float shininess, vec3 norm)
{
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(light.dir, norm); 
    float diffFactor = max(dot(-light.dir, norm), 0.f);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.f), shininess);
    vec3 ambientCol = dif * light.ambient;
    vec3 diffuse = dif * diffFactor;
    vec3 specular = spec * specFactor;
    return (diffuse + specular) * light.color + ambientCol;
}
vec3 computeSpotLight(in SpotLight light, vec3 dif, vec3 spec, float shininess, vec3 norm)
{
    vec3 ambientCol = dif * light.ambient;

    vec3 lightDir = normalize(light.pos - fragPos);
    float theta = dot(lightDir, normalize(-light.dir));
    float epsilon = light.cutOff - light.outerCutOff;
    if (epsilon > light.outerCutOff)
        return ambientCol;

    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.f, 1.0);

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float diffFactor = max(dot(lightDir, norm), 0.f);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.f), shininess);
    vec3 diffuse = dif * diffFactor;
    vec3 specular = spec * specFactor;
    vec3 lightColor = (diffuse + specular) * light.color * intensity + ambientCol;

    float dist = distance(light.pos, fragPos);
    float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));  

    return lightColor * attenuation;
}

vec3 computeNormal(in vec3 normal)
{
    vec3 realNormal = normal * 2.f - 1.f;
    return normalize(TBN * realNormal);
}

uniform PointLight pointLightSources[32];
uniform DirectionalLight dirLightSources[32];
uniform SpotLight spotLightSources[32];
uniform int pointLightsCount, dirLightsCount, spotLightsCount;
uniform sampler2D textures[16];
uniform vec3 sceneAmbient;
uniform samplerCube skybox;

const float gamma = 2.2f;

const float near = 0.1f;
const float far = 100.f;

float getDepth()
{
    float z = gl_FragCoord.z * 2.0 - 1.0;
    float depth = (2.0 * near * far) / (far + near - z * (far - near));
    return depth / far;
}

void main()
{
    vec3 light = vec3(0.f);
    vec3 diffuse = texture(textures[0], fragUv).rgb;
    vec3 specular = vec3(0.07f);
    vec3 realNormal = computeNormal(texture(textures[1], fragUv).rgb);
    vec4 material = texture(textures[2], fragUv);
    float shininess = 4;
    for (int i = 0; i < pointLightsCount; i++)
        light += computePointLight(pointLightSources[i], diffuse, specular, shininess, realNormal);
    for (int i = 0; i < dirLightsCount; i++)
        light += computeDirectionalLight(dirLightSources[i], diffuse, specular, shininess, realNormal);
    for (int i = 0; i < spotLightsCount; i++)
        light += computeSpotLight(spotLightSources[i], diffuse, specular, shininess, realNormal);

    light += sceneAmbient * diffuse;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(viewDir, realNormal);
    vec3 coord = vec3(-reflectDir.x, reflectDir.z, -reflectDir.y);
    vec3 reflectedColor = texture(skybox, coord).rgb;
    float mixValue = (1.f - material.b) * material.g;
    vec3 pixelColor = mix(light, reflectedColor, 1.0f);
    //vec3 finalColor = mix(pixelColor, vec3(0.2f, 0.3f, 0.3f), getDepth());

    FragColor = vec4(pixelColor, 1.f);
}
