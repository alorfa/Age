#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;
layout (location = 3) in vec3 inTangent;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    computeTBN(inNormal, inTangent);
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

uniform sampler2D textures[8];
uniform sampler2D baseColorMap, normalMap, roughnessMap, metalnessMap;

void fragmentControl()
{
    age_base_color = texture(baseColorMap, fragUv).rgb;
    age_specular = vec3(0.07f);
    age_normal = computeNormal(texture(normalMap, fragUv).rgb);
    //age_normal = normalize(fragTBN[2]);
    vec4 material = texture(roughnessMap, fragUv);
    age_roughness = material.g;
    age_shininess = 4.f;
    age_metalness = material.b;
}
#define AGE_LIGHT_MODE_PBR

#endif