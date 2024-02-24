#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;
layout (location = 3) in vec3 inTangent;

vec4 vertexProcess()
{
    fragUv = inUv;
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    fragTBN = computeTBN(inNormal, inTangent, mat3(model));
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

uniform sampler2D baseColorMap, normalMap, roughnessMap, metalnessMap;

void setup()
{
    material.base_color = texture(baseColorMap, fragUv).rgb;
    material.normal = computeNormal(texture(normalMap, fragUv).rgb);
    //material.normal = normalize(fragTBN[2]);
    vec4 m = texture(roughnessMap, fragUv);
    material.roughness = m.g;
    material.metallic = m.b;
}

#endif