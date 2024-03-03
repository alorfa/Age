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

uniform float metallic, roughness;

void setup()
{
    material.base_color = vec3(1., 0.766, 0.336);
    //material.base_color = vec3(0.1, 0.4, 0.9);
    material.normal = normalize(fragTBN[2]);
    material.roughness = roughness;
    material.metallic = metallic;
}

#endif