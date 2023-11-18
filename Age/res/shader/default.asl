#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    fragTBN[2] = mat3(model) * inNormal;
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

uniform sampler2D baseColorMap;

void fragmentControl()
{
    age_base_color = texture(baseColorMap, fragUv).rgb;
    //age_specular = age_base_color;
    age_normal = normalize(fragTBN[2]);
    //age_metalness = 0.3;
    //age_shininess = 64.f;
    //age_base_color = vec3(1.);
    age_metalness = 0.1;
    age_roughness = 0.35;
}
#define AGE_LIGHT_MODE_PBR

#endif