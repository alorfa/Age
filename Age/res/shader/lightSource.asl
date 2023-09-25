#ifdef AGE_VERTEX

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model, view, projection;

vec4 vertexProcess()
{
    return projection * view * model * vec4(position, 1.0);
}
#endif
#ifdef AGE_FRAGMENT



#endif