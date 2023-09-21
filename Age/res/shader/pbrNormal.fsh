//defines
#define AGE_FRAGMENT
#define AGE_RENDERING_MODE_FORWARD
#define AGE_MAX_DIR_LIGHTS 1
#define AGE_MAX_POINT_LIGHTS 2
#define AGE_MAX_SPOT_LIGHTS 1

//bindings
layout(location = 0) out vec4 age_FragColor;
layout(location = 1) out vec4 age_FragColor1;
layout(location = 2) out vec4 age_FragColor2;
layout(location = 3) out vec4 age_FragColor3;

//begin include

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

in vec2 fragUv;
in vec3 fragPos;
in mat3 fragTBN;

uniform vec3 cameraPos;

uniform vec3 sceneAmbient;
uniform samplerCube skybox;

const float near = 0.1f;
const float far = 100.f;

vec3 age_emission, age_base_color, age_specular, age_normal;
float age_metalness, age_roughness, age_shininess;

#if defined(AGE_MAX_POINT_LIGHTS) && AGE_MAX_POINT_LIGHTS != 0
uniform PointLight pointLightSources[AGE_MAX_POINT_LIGHTS];
#endif
#if defined(AGE_MAX_DIR_LIGHTS) && AGE_MAX_DIR_LIGHTS != 0
uniform DirectionalLight dirLightSources[AGE_MAX_DIR_LIGHTS];
#endif
#if defined(AGE_MAX_SPOT_LIGHTS) && AGE_MAX_SPOT_LIGHTS != 0
uniform SpotLight spotLightSources[AGE_MAX_SPOT_LIGHTS];
#endif

vec4 sampleCubemap(samplerCube cubemap, vec3 texCoord)
{
    vec3 coord = vec3(texCoord.x, -texCoord.z, texCoord.y);
    return texture(cubemap, coord);
}

vec3 phong_computePointLight(in PointLight light, vec3 dif, vec3 spec, float shininess, vec3 norm)
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

vec3 phong_computeDirectionalLight(in DirectionalLight light, vec3 dif, vec3 spec, float shininess, vec3 norm)
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
vec3 phong_computeSpotLight(in SpotLight light, vec3 dif, vec3 spec, float shininess, vec3 norm)
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
    return normalize(fragTBN * realNormal);
}
float getDepth()
{
    float z = gl_FragCoord.z * 2.0 - 1.0;
    float depth = (2.0 * near * far) / (far + near - z * (far - near));
    return depth / far;
}

void pbr_paintOver()
{
    vec3 viewDir = normalize(fragPos - cameraPos);
    vec3 reflectDir = reflect(viewDir, age_normal);
    vec3 reflectedColor = sampleCubemap(skybox, reflectDir).rgb;

    age_FragColor = vec4(reflectedColor, 1.f);
}
void phong_paintOver()
{
    vec3 light = vec3(0.f);
    for (int i = 0; i < AGE_MAX_POINT_LIGHTS; i++)
        light += phong_computePointLight(pointLightSources[i], age_base_color, age_specular, age_shininess, age_normal);
    for (int i = 0; i < AGE_MAX_DIR_LIGHTS; i++)
        light += phong_computeDirectionalLight(dirLightSources[i], age_base_color, age_specular, age_shininess, age_normal);
    for (int i = 0; i < AGE_MAX_SPOT_LIGHTS; i++)
        light += phong_computeSpotLight(spotLightSources[i], age_base_color, age_specular, age_shininess, age_normal);
    light += sceneAmbient * age_base_color;

    age_FragColor = vec4(light, 1.f);
}

//end include

//user's code

uniform sampler2D textures[16];

void control()
{
    age_base_color = texture(textures[0], fragUv).rgb;
    age_specular = vec3(0.07f);
    age_normal = computeNormal(texture(textures[1], fragUv).rgb);
    //age_normal = normalize(fragTBN[2]);
    vec4 material = texture(textures[2], fragUv);
    age_roughness = material.g;
    age_shininess = 4.f;
    age_metalness = material.b;
}
#define AGE_LIGHT_MODE_PHONG

//main
void age_paintOver()
{
    #if defined(AGE_LIGHT_MODE_FORCE)
    force_paintOver();
    #elif defined(AGE_LIGHT_MODE_CUSTOM)
    paintOver();
    #elif defined(AGE_RENDERING_MODE_FORWARD) && defined(AGE_LIGHT_MODE_PBR)
    pbr_paintOver();
    #elif defined(AGE_RENDERING_MODE_FORWARD) && defined(AGE_LIGHT_MODE_PHONG)
    phong_paintOver();
    #else
    age_FragColor = vec4(1.f);
    #endif
}

void main()
{
    control();
    age_paintOver();
}
