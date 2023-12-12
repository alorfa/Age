#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    fragTBN = computeTBN(inNormal, inTangent, mat3(model));
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

void fragmentControl()
{
    age_base_color = vec3(1.);
    age_normal = normalize(fragTBN[2]);
    age_roughness = 0.15;
    age_metalness = 0.99;
}
#define AGE_LIGHT_MODE_PBR

#endif