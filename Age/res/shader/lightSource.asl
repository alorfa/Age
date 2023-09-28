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

void fragmentControl()
{
    age_base_color = emission;
}
#define AGE_LIGHT_MODE_CUSTOM
void paintOver()
{
	age_FragColor = vec4(age_base_color, 1.f);
}

#endif