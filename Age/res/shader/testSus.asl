#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;
layout (location = 3) in vec3 inTangent;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    fragTBN[2] = mat3(model) * inNormal;
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

uniform float metalic, roughness;

void fragmentControl()
{
    age_base_color = vec3(1.);
    age_normal = normalize(fragTBN[2]);
    age_roughness = metalic;
    age_metalness = roughness;
}
#define AGE_LIGHT_MODE_PBR

#endif