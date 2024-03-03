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

uniform vec3 color;

void setup()
{
    material.base_color = color;
    material.normal = normalize(fragTBN[2]);
    material.roughness = 0.08;
    material.metallic = 0.;
    material.alpha = 0.3;
    material.reflectivity = 0.2;
}

#endif