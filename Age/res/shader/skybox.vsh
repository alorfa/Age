#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 view, projection;

out vec3 fragUv;

void main()
{
	fragUv = inPosition;
    gl_Position = projection * view * vec4(inPosition, 1.f);
    //gl_Position.z = 0.f;
}
