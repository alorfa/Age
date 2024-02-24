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

void setup()
{
    material.base_color = texture(baseColorMap, fragUv).rgb;
    material.normal = normalize(fragTBN[2]);
    material.metallic = 0.1;
    material.roughness = 0.05;
}

#endif