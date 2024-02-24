#ifdef AGE_VERTEX

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

vec4 vertexProcess()
{
    fragPos = (model * vec4(inPosition, 1.f)).xyz;
    fragTBN[2] = mat3(model) * inNormal;
    return projection * view * model * vec4(inPosition, 1.0);
}

#endif
#ifdef AGE_FRAGMENT

uniform vec3 emission;

void setup()
{
    material.normal = normalize(fragTBN[2]);
    material.roughness = 0.1;
    material.metallic = 0.;
    material.emission = emission;
}

#endif